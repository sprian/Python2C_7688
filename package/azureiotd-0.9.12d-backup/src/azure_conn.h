#pragma once
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { EVI_AUTH_SAS = 0, EVI_AUTH_X509 = 1 } evi_auth_mode_t;
typedef struct {
    char host[256];
    char device_id[128];
    evi_auth_mode_t auth;
    char sas_key[256]; int sas_ttl;
    char x509_cert[256]; char x509_key[256]; char x509_key_pass[128];
    char sni_host[256]; char ca_file[256]; int keepalive;
} evi_azure_cfg_t;
typedef struct { int connected; } evi_azure_runtime_t;
int azure_conn_init(evi_azure_runtime_t* rt, const evi_azure_cfg_t* cfg);
char* azure_sas_build_token(const char* host, const char* device_id, const char* b64_key, int ttl_secs);
int azure_conn_run(evi_azure_runtime_t* rt, const evi_azure_cfg_t* cfg);
#ifdef __cplusplus
}
#endif
