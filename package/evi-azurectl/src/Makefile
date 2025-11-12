CC ?= gcc
CFLAGS ?= -Os -Wall -Wextra -std=gnu99 -fdata-sections -ffunction-sections
LDFLAGS ?= -Wl,--gc-sections
OBJS = main.o serial_posix.o uci_shim.o usb_dongle.o utils.o
all: evi-azurectl
evi-azurectl: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -lpthread
clean:
	rm -f $(OBJS) evi-azurectl
