#pragma once
#ifdef __cplusplus
extern "C" {
#endif
void evi_charge_preparing(void);
void evi_charge_auth_wait_char(void);
void evi_charge_auth_wait_eng(void);
void evi_charge_plugin_wait_char(void);
void evi_charge_plugin_wait_eng(void);
void evi_charge_get_mvalue(void);
void evi_charge_disp_complete(void);
void evi_charge_server_conn(void);
void evi_charge_server_conn_failed(void);
void evi_charge_server_conn_lost(void);
#ifdef __cplusplus
}
#endif
