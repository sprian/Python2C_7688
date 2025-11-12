#include "../include/evi_caption.h"
#include "evi_charge.h"
#include "../include/evi_hal.h"
#include "evi_mutex.h"
#include <string.h>
#define DISPLAY_CLEAR 0x01
#define DISPLAY_ON 0x0C
#define BASIC_FUNC 0x38

enum{SCN_PREP=100,SCN_AUTH_ZH,SCN_AUTH_EN,SCN_PLUGIN_ZH,SCN_PLUGIN_EN,SCN_SRV_CONN,SCN_SRV_FAIL,SCN_SRV_LOST,SCN_MVAL,SCN_DONE};
static struct{int id;char a[48],b[48];}u2={0,"",""};
static inline int eq(const char* x,const char* y){ if(!x&&!y) return 1; if(!x||!y) return 0; return strcmp(x,y)==0; }
static int need2(int id,const char* a,const char* b){ if(u2.id!=id) return 1; if(!eq(a,u2.a)) return 1; if(!eq(b,u2.b)) return 1; return 0; }
static void keep2(int id,const char* a,const char* b){ u2.id=id; u2.a[0]=u2.b[0]=0; if(a){strncpy(u2.a,a,sizeof(u2.a)-1);u2.a[sizeof(u2.a)-1]=0;} if(b){strncpy(u2.b,b,sizeof(u2.b)-1);u2.b[sizeof(u2.b)-1]=0;} }
static void begin(void){ hal_write_instr(BASIC_FUNC); hal_delay_us(72); hal_write_instr(DISPLAY_CLEAR); hal_delay_us(1600); hal_write_instr(DISPLAY_ON); hal_delay_us(72);} 
void evi_caption_preparing(void){ evi_lock(); if(!need2(SCN_PREP,0,0)){evi_unlock();return;} begin(); evi_charge_preparing(); keep2(SCN_PREP,0,0); evi_unlock(); }
void evi_caption_authWaiting(int s){ evi_lock(); int id=(s%2)==0?SCN_AUTH_ZH:SCN_AUTH_EN; if(!need2(id,0,0)){evi_unlock();return;} begin(); if(id==SCN_AUTH_ZH) evi_charge_auth_wait_char(); else evi_charge_auth_wait_eng(); keep2(id,0,0); evi_unlock(); }
void evi_caption_plugInWaiting(int s){ evi_lock(); int id=(s%2)==0?SCN_PLUGIN_ZH:SCN_PLUGIN_EN; if(!need2(id,0,0)){evi_unlock();return;} begin(); if(id==SCN_PLUGIN_ZH) evi_charge_plugin_wait_char(); else evi_charge_plugin_wait_eng(); keep2(id,0,0); evi_unlock(); }
void evi_caption_serverConn(void){ evi_lock(); if(!need2(SCN_SRV_CONN,0,0)){evi_unlock();return;} begin(); evi_charge_server_conn(); keep2(SCN_SRV_CONN,0,0); evi_unlock(); }
void evi_caption_serverConnFailed(void){ evi_lock(); if(!need2(SCN_SRV_FAIL,0,0)){evi_unlock();return;} begin(); evi_charge_server_conn_failed(); keep2(SCN_SRV_FAIL,0,0); evi_unlock(); }
void evi_caption_serverConnLost(void){ evi_lock(); if(!need2(SCN_SRV_LOST,0,0)){evi_unlock();return;} begin(); evi_charge_server_conn_lost(); keep2(SCN_SRV_LOST,0,0); evi_unlock(); }
void evi_caption_getMValue(const char* a,const char* b){ evi_lock(); if(!need2(SCN_MVAL,a,b)){evi_unlock();return;} begin(); evi_charge_get_mvalue(); keep2(SCN_MVAL,a,b); evi_unlock(); }
void evi_caption_dispComplete(const char* a,const char* b){ evi_lock(); if(!need2(SCN_DONE,a,b)){evi_unlock();return;} begin(); evi_charge_disp_complete(); keep2(SCN_DONE,a,b); evi_unlock(); }
