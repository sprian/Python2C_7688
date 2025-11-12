CC ?= gcc
CFLAGS ?= -Os -Wall -Wextra -std=gnu99 -fdata-sections -ffunction-sections
LDFLAGS ?= -Wl,--gc-sections
OBJS = gpio_sysfs.o lcd_hw.o lcdsvc.o
CLI_OBJS = lcdctl.o
all: lcdsvc lcdctl
lcdsvc: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
lcdctl: $(CLI_OBJS)
	$(CC) $(CLI_OBJS) -o $@ $(LDFLAGS)
clean:
	rm -f $(OBJS) $(CLI_OBJS) lcdsvc lcdctl
