#pragma once
#include <stdint.h>

int gpio_export(int gpio);
int gpio_unexport(int gpio);
int gpio_set_dir(int gpio, int is_out);
int gpio_write(int gpio, int value);
int gpio_read(int gpio);
int gpio_write_fast(const char *path, int value); // internal helper exposed for tests
