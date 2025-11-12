#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
int serial_open(const char* dev, int baud);
int serial_close(int fd);
ssize_t serial_read(int fd, void* buf, size_t n);
ssize_t serial_write(int fd, const void* buf, size_t n);
int serial_flush(int fd);
static uint16_t crc16(const uint8_t* data, size_t len){
    uint16_t crc = 0xFFFF;
    for(size_t i=0;i<len;i++){
        crc ^= data[i];
        for(int j=0;j<8;j++){
            if (crc & 1) crc = (crc >> 1) ^ 0xA001;
            else crc >>= 1;
        }
    }
    return crc;
}
int mb_read_holding_regs(const char* dev, int baud, uint8_t id, uint16_t addr, uint16_t count, uint8_t* out, size_t outcap, size_t* outn){
    int fd = serial_open(dev, baud); if(fd < 0) return -1;
    uint8_t req[8]; req[0]=id; req[1]=0x03; req[2]=addr>>8; req[3]=addr&0xFF; req[4]=count>>8; req[5]=count&0xFF;
    uint16_t c=crc16(req,6); req[6]=c&0xFF; req[7]=c>>8;
    serial_flush(fd);
    if (serial_write(fd, req, sizeof(req)) != sizeof(req)){ serial_close(fd); return -2; }
    usleep(12000);
    uint8_t hdr[3]; ssize_t n = serial_read(fd, hdr, 3);
    if(n != 3){ serial_close(fd); return -3; }
    if(hdr[0]!=id || hdr[1]!=0x03){ serial_close(fd); return -4; }
    uint8_t bytecnt = hdr[2];
    if(bytecnt+2 > outcap){ serial_close(fd); return -5; }
    ssize_t m = serial_read(fd, out, bytecnt + 2);
    if(m != bytecnt + 2){ serial_close(fd); return -6; }
    uint16_t crc_calc = 0xFFFF;
    for (int i=0;i<3;i++){ crc_calc ^= (&hdr[0])[i]; for(int j=0;j<8;j++){ crc_calc = (crc_calc & 1) ? ((crc_calc >> 1) ^ 0xA001) : (crc_calc >> 1); } }
    for (int i=0;i<bytecnt;i++){ crc_calc ^= out[i]; for(int j=0;j<8;j++){ crc_calc = (crc_calc & 1) ? ((crc_calc >> 1) ^ 0xA001) : (crc_calc >> 1); } }
    uint16_t crc_recv = out[bytecnt] | ((uint16_t)out[bytecnt+1] << 8);
    serial_close(fd);
    if (crc_calc != crc_recv) return -8;
    *outn = bytecnt; return 0;
}
