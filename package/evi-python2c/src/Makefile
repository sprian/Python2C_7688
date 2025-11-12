CC ?= gcc
CFLAGS ?= -Os -Wall -Wextra -std=gnu99 -fdata-sections -ffunction-sections
LDFLAGS ?= -Wl,--gc-sections
all: evi-python2c
evi-python2c: main.o
	$(CC) $^ -o $@ $(LDFLAGS) -lpthread
clean:
	rm -f *.o evi-python2c
