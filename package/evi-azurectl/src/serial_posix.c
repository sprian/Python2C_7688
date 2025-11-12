#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

static speed_t to_baud(int b){
    switch(b){ case 9600: return B9600; case 19200: return B19200; case 38400: return B38400;
               case 57600: return B57600; case 115200: return B115200; default: return B115200; }
}
int serial_open(const char* dev, int baud){
    int fd = open(dev, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(fd < 0) return -1;
    struct termios tio; memset(&tio,0,sizeof(tio)); tcgetattr(fd,&tio);
    cfsetospeed(&tio,to_baud(baud)); cfsetispeed(&tio,to_baud(baud));
    tio.c_cflag = (CS8|CLOCAL|CREAD); tio.c_iflag = 0; tio.c_oflag = 0; tio.c_lflag = 0;
    tio.c_cc[VMIN]=0; tio.c_cc[VTIME]=2; tcsetattr(fd,TCSANOW,&tio);
    int flags = fcntl(fd, F_GETFL, 0); fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
    return fd;
}
int serial_close(int fd){ return close(fd); }
ssize_t serial_read(int fd, void* buf, size_t n){ return read(fd, buf, n); }
ssize_t serial_write(int fd, const void* buf, size_t n){ return write(fd, buf, n); }
int serial_flush(int fd){ return tcflush(fd, TCIOFLUSH); }
