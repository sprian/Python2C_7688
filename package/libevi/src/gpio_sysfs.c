#include "gpio_sysfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static int write_str(const char *path, const char *s) {
    int fd = open(path, O_WRONLY | O_CLOEXEC);
    if (fd < 0) return -1;
    ssize_t n = write(fd, s, strlen(s));
    int err = (n < 0 || (size_t)n != strlen(s)) ? -1 : 0;
    close(fd);
    return err;
}

int gpio_export(int gpio) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d\n", gpio);
    // Try export, ignore EBUSY if already exported
    int r = write_str("/sys/class/gpio/export", buf);
    if (r != 0) {
        // Already exported? ok.
        // Give it a moment to appear
        usleep(1000);
    }
    return 0;
}

int gpio_unexport(int gpio) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d\n", gpio);
    return write_str("/sys/class/gpio/unexport", buf);
}

int gpio_set_dir(int gpio, int is_out) {
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpio);
    return write_str(path, is_out ? "out\n" : "in\n");
}

int gpio_write_fast(const char *path, int value) {
    int fd = open(path, O_WRONLY | O_CLOEXEC);
    if (fd < 0) return -1;
    const char c = value ? '1' : '0';
    ssize_t n = write(fd, &c, 1);
    int err = (n != 1) ? -1 : 0;
    close(fd);
    return err;
}

int gpio_write(int gpio, int value) {
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    return gpio_write_fast(path, value);
}

int gpio_read(int gpio) {
    char path[128];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0) return -1;
    char c = 0;
    ssize_t n = read(fd, &c, 1);
    close(fd);
    if (n != 1) return -1;
    return (c == '1') ? 1 : 0;
}
