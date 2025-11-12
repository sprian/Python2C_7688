#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "include/gpio_sysfs.h"

static int write_str(const char* path, const char* s){
    int fd = open(path, O_WRONLY);
    if (fd < 0) return -1;
    ssize_t n = write(fd, s, strlen(s));
    close(fd);
    return (n == (ssize_t)strlen(s)) ? 0 : -1;
}
int gpio_export(int pin){ char buf[16]; snprintf(buf, sizeof(buf), "%d", pin); return write_str("/sys/class/gpio/export", buf); }
int gpio_unexport(int pin){ char buf[16]; snprintf(buf, sizeof(buf), "%d", pin); return write_str("/sys/class/gpio/unexport", buf); }
int gpio_direction(int pin, int is_output){ char path[128]; snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin); return write_str(path, is_output ? "out" : "in"); }
int gpio_write(int pin, int val){ char path[128]; snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin); return write_str(path, val ? "1" : "0"); }
int gpio_open_handle(int pin, int is_output){ (void)is_output; char path[128]; snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin); int fd = open(path, O_WRONLY); if (fd < 0) return -1; return fd; }
int gpio_write_fd(int fd, int val){ const char* s = val ? "1":"0"; if (lseek(fd, 0, SEEK_SET) < 0) return -1; return (write(fd, s, 1) == 1) ? 0 : -1; }
void gpio_close_fd(int fd){ if (fd >= 0) close(fd); }
int gpio_read(int pin){ char path[128], buf[8]; snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin); int fd = open(path, O_RDONLY); if (fd < 0) return -1; ssize_t n = read(fd, buf, sizeof(buf)-1); close(fd); if (n <= 0) return -1; buf[n]=0; return (buf[0]=='1')?1:0; }
int gpio_write_fast(int pin, int val){ return gpio_write(pin, val); }
