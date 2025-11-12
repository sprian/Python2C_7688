#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include <termios.h>

int serial_open(const char* dev, int baud);
int serial_close(int fd);
ssize_t serial_read(int fd, void* buf, size_t n);
ssize_t serial_write(int fd, const void* buf, size_t n);
int serial_flush(int fd);

static const uint8_t NORMAL_CLOSE[4] = {0,1,1,1};

static int find_ttyUSB(char* path, size_t len){
    glob_t g; if (glob("/dev/ttyUSB*", 0, NULL, &g)!=0) return -1;
    if (g.gl_pathc < 1){ globfree(&g); return -1; }
    strncpy(path, g.gl_pathv[0], len-1); path[len-1]=0; globfree(&g); return 0;
}

// Send A0 setting then 0xFF query; ensure one-question-one-answer buffers
int usb_set_port(const char* act, int port){
    if (!act || port < 1 || port > 4) return -10;

    char dev[64];
    if (find_ttyUSB(dev, sizeof(dev))<0) return -1;

    int fd = serial_open(dev, 9600);
    if (fd<0) return -2;

    // Clear RX/TX
    tcflush(fd, TCIOFLUSH);

    uint8_t msg[4] = {0};
    uint8_t val = (!strcmp(act,"On")) ? 1 : 0;
    uint8_t real = val ^ NORMAL_CLOSE[port-1];
    msg[0]=0xA0; msg[1]=(uint8_t)port; msg[2]=real; msg[3]=msg[0]+msg[1]+msg[2];

    if (serial_write(fd, msg, 4) != 4){ serial_close(fd); return -3; }

    // Settle
    usleep(500000);

    // Query 0xFF
    uint8_t inq=0xFF;
    if (serial_write(fd, &inq, 1)!=1){ serial_close(fd); return -4; }

    // Robust read: accumulate until >=40 bytes or 4 lines observed
    uint8_t resp[128]; memset(resp, 0, sizeof(resp));
    ssize_t total = 0;
    int lines = 0;
    const int NEED_MIN = 40;
    const int MAX_WAIT_LOOPS = 20; // ~1s

    for (int tries=0; tries<MAX_WAIT_LOOPS; ++tries){
        ssize_t n = serial_read(fd, resp + total, sizeof(resp) - total);
        if (n > 0){
            for (ssize_t i=0; i<n; ++i){ if (resp[total + i] == '\n') lines++; }
            total += n;
            if (total >= NEED_MIN && lines >= 4) break;
            if (total >= (ssize_t)sizeof(resp)) break;
            usleep(50000);
        } else {
            if (total >= NEED_MIN) break;
            usleep(50000);
        }
    }

    // Clear RX for next cycle
    tcflush(fd, TCIFLUSH);

    serial_close(fd);
    return (total >= NEED_MIN) ? 0 : -5;
}
