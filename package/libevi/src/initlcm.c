// initlcm.c — HD44780-like LCM init sequence for MT7688, rewritten from Python mraa to pure sysfs GPIO.
// Build with OpenWrt 15.05.1 (ramips/mt7688) toolchain.
// If your board's Linux GPIO numbers differ, adjust the GPIO_* and DB* defines below.
//
// Original Python used mraa logical pins: RS=18, RW=17, EN=37, DB7..DB0 = 5,4,3,2,1,0,46,45.
#include "gpio_sysfs.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

// ---- Pin mapping (edit these if needed) ----
#define GPIO_RS  18
#define GPIO_RW  17
#define GPIO_EN  37

#define GPIO_DB7 5
#define GPIO_DB6 4
#define GPIO_DB5 3
#define GPIO_DB4 2
#define GPIO_DB3 1
#define GPIO_DB2 0
#define GPIO_DB1 46
#define GPIO_DB0 45

static int all_pins[] = {
    GPIO_RS, GPIO_RW, GPIO_EN,
    GPIO_DB7, GPIO_DB6, GPIO_DB5, GPIO_DB4,
    GPIO_DB3, GPIO_DB2, GPIO_DB1, GPIO_DB0
};

static inline void delay_100us(void) {
    // Sleep ~100 microseconds
    usleep(100);
}

static inline void delay_10ms(void) {
    usleep(10 * 1000);
}

static void set_data_bus(unsigned char v) {
    gpio_write(GPIO_DB7, (v >> 7) & 1);
    gpio_write(GPIO_DB6, (v >> 6) & 1);
    gpio_write(GPIO_DB5, (v >> 5) & 1);
    gpio_write(GPIO_DB4, (v >> 4) & 1);
    gpio_write(GPIO_DB3, (v >> 3) & 1);
    gpio_write(GPIO_DB2, (v >> 2) & 1);
    gpio_write(GPIO_DB1, (v >> 1) & 1);
    gpio_write(GPIO_DB0, (v >> 0) & 1);
}

static void write_cmd(unsigned char v) {
    // RS=0, RW=0, EN=1, set data, EN=0 — mirrors the Python logic
    gpio_write(GPIO_RS, 0);
    gpio_write(GPIO_RW, 0);
    gpio_write(GPIO_EN, 1);
    set_data_bus(v);
    gpio_write(GPIO_EN, 0);
}

static int setup_pins(void) {
    for (unsigned i = 0; i < sizeof(all_pins)/sizeof(all_pins[0]); ++i) {
        int p = all_pins[i];
        if (gpio_export(p) != 0) { perror("export"); }
        // give kernel a moment to create the node
        usleep(1000);
        if (gpio_set_dir(p, 1) != 0) { perror("direction"); }
        if (gpio_write(p, 0) != 0) { /* ignore */ }
    }
    return 0;
}

int main(void) {
    if (setup_pins() != 0) {
        fprintf(stderr, "Failed to setup pins (need root?).\n");
        return 1;
    }

    // Initialization sequence (8-bit mode), matching the Python version:
    // Function Set (0x30), twice
    write_cmd(0x30);
    delay_100us();
    write_cmd(0x30);
    delay_100us();

    // Display ON (0x0C)
    write_cmd(0x0C);
    delay_100us();

    // Display Clear (0x01)
    write_cmd(0x01);
    delay_10ms(); // Python slept 10ms here

    // Entry Mode Set (0x06)
    write_cmd(0x06);

    return 0;
}
