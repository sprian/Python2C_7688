#ifndef DPS_CERTIFICATE_H
#define DPS_CERTIFICATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char serial_no[64];
    char cert_path[256];
    char key_path[256];
    char full_chain_path[256];
} dps_cert_manager_t;

// 函數聲明
int dps_generate_certificates(dps_cert_manager_t *cert_mgr);
int dps_check_certificates_exist(const char *serial_no);
char* dps_get_serial_number(void);

#endif
