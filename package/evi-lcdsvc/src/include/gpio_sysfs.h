#ifndef GPIO_SYSFS_H
#define GPIO_SYSFS_H
#include <stddef.h>
int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int pin, int is_output);
int gpio_write(int pin, int val);
int gpio_read(int pin);
int gpio_write_fast(int pin, int val);
int gpio_open_handle(int pin, int is_output);
int gpio_write_fd(int fd, int val);
void gpio_close_fd(int fd);
#endif
