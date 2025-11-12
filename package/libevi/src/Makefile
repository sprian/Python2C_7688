CC ?= gcc
CFLAGS ?= -Os -fPIC -std=gnu99 -Wall -Wextra -pthread
LDFLAGS ?= -shared -pthread
OBJS = evi_version.o evi_mutex.o evi_caption.o evi_charge.o evi_charge_router.o evi_hal_mt7688_gpio.o gpio_sysfs.o
all: libevi.so libevi_caption.so initlcm
%.o: %.c
	$(CC) $(CFLAGS) -I../include -c $< -o $@
libevi.so: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
libevi_caption.so: libevi.so
	ln -sf libevi.so libevi_caption.so
initlcm: initlcm.c gpio_sysfs.o
	$(CC) -Os -std=gnu99 -Wall -Wextra -o $@ initlcm.c gpio_sysfs.o
clean:
	rm -f *.o libevi.so libevi_caption.so initlcm
