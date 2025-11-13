#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int slave_id;
    char device_path[64];
    int baud_rate;
    int initialized;
} modbus_rtu_t;

// 函數聲明
int modbus_rtu_init(modbus_rtu_t *modbus, const char *device, int baud_rate);
int modbus_read_register(modbus_rtu_t *modbus, int address, int byte_count, unsigned char *buffer);
int modbus_write_register(modbus_rtu_t *modbus, int address, int value);
void modbus_rtu_cleanup(modbus_rtu_t *modbus);

#endif
