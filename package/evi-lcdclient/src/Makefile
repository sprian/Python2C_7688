CC ?= gcc
CFLAGS ?= -Os -Wall -Wextra -std=gnu99 -fPIC
LDFLAGS ?= -shared -Wl,--gc-sections
OBJS = evi_lcd.o
all: libevi_lcd.so
libevi_lcd.so: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) -lpthread
clean:
	rm -f $(OBJS) libevi_lcd.so
