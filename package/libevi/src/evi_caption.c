#include "../include/evi_caption.h"
#include "../include/evi_hal.h"
#include "evi_mutex.h"
#include <string.h>
#define DISPLAY_CLEAR 0x01
#define DISPLAY_ON 0x0C
#define BASIC_FUNC 0x38

enum{SCN_NONE=0,SCN_INITIAL,SCN_SYSLOAD,SCN_INFO1,SCN_INFO2,SCN_INFO3,SCN_INFO4,SCN_STANDBY};
static struct{int id,inited,extra;char a[48],b[48];}ui={0,0,0,"",""};
static inline int streq(const char* x,const char* y){ if(!x&&!y) return 1; if(!x||!y) return 0; return strcmp(x,y)==0; }
static int need(int id,const char* a,const char* b,int e){ if(!ui.inited){ui.inited=1;return 1;} if(ui.id!=id) return 1; if(!streq(a,ui.a)) return 1; if(!streq(b,ui.b)) return 1; if(ui.extra!=e) return 1; return 0; }
static void keep(int id,const char* a,const char* b,int e){ ui.id=id; ui.extra=e; ui.a[0]=ui.b[0]=0; if(a){strncpy(ui.a,a,sizeof(ui.a)-1);ui.a[sizeof(ui.a)-1]=0;} if(b){strncpy(ui.b,b,sizeof(ui.b)-1);ui.b[sizeof(ui.b)-1]=0;} }
static void begin(void){ hal_write_instr(BASIC_FUNC); hal_delay_us(72); hal_write_instr(DISPLAY_CLEAR); hal_delay_us(1600); hal_write_instr(DISPLAY_ON); hal_delay_us(72);} 
static void big5(unsigned char hi,unsigned char lo){ hal_write_graphic(hi); hal_write_graphic(lo); hal_delay_us(72);} 
const char* evi_caption_version(void){ return "libevi/0.9.0"; }
void evi_caption_initial(void){ evi_lock(); if(!need(SCN_INITIAL,0,0,0)){evi_unlock();return;} begin(); hal_move_ddram(0); hal_write_letters(" Charger Initialization"); hal_move_ddram(15); big5(0xA5,0x52); big5(0xB9,0x71); big5(0xBE,0xB9); big5(0xAA,0xEC); big5(0xA9,0x6C); big5(0xA4,0xC6); keep(SCN_INITIAL,0,0,0); evi_unlock(); }
void evi_caption_system_loading(void){ evi_lock(); if(!need(SCN_SYSLOAD,0,0,0)){evi_unlock();return;} begin(); hal_move_ddram(2); hal_write_letters("System Loading..."); hal_move_ddram(15); big5(0xA8,0x74); big5(0xB2,0xCE); big5(0xB8,0xFC); big5(0xA4,0x4A); big5(0xA4,0xA4); keep(SCN_SYSLOAD,0,0,0); evi_unlock(); }
void evi_caption_info1(const char* m,const char* s){ evi_lock(); if(!need(SCN_INFO1,m,s,0)){evi_unlock();return;} begin(); hal_move_ddram(0); hal_write_letters("MCU Ver:"); hal_move_ddram(5); hal_write_letters(m?m:""); hal_move_ddram(12); hal_write_letters("SoC Ver:"); hal_move_ddram(17); hal_write_letters(s?s:""); keep(SCN_INFO1,m,s,0); evi_unlock(); }
void evi_caption_info2(const char* sn,const char* pn){ evi_lock(); if(!need(SCN_INFO2,sn,pn,0)){evi_unlock();return;} begin(); hal_move_ddram(0); hal_write_letters("SN:"); hal_move_ddram(2); hal_write_letters(sn?sn:""); hal_move_ddram(12); hal_write_letters("Prod. Name:"); hal_move_ddram(18); hal_write_letters(pn?pn:""); keep(SCN_INFO2,sn,pn,0); evi_unlock(); }
void evi_caption_info3(const char* mv,const char* mc){ evi_lock(); if(!need(SCN_INFO3,mv,mc,0)){evi_unlock();return;} begin(); hal_move_ddram(0); hal_write_letters("Meter Ver:"); hal_move_ddram(6); hal_write_letters(mv?mv:""); hal_move_ddram(12); hal_write_letters("Max Current:"); hal_move_ddram(19); hal_write_letters(mc?mc:""); keep(SCN_INFO3,mv,mc,0); evi_unlock(); }
void evi_caption_info4(const char* m2,const char* c2){ evi_lock(); if(!need(SCN_INFO4,m2,c2,0)){evi_unlock();return;} begin(); hal_move_ddram(0); hal_write_letters("Meter 2 Ver:"); hal_move_ddram(7); hal_write_letters(m2?m2:""); hal_move_ddram(12); hal_write_letters("Max Current:"); hal_move_ddram(19); hal_write_letters(c2?c2:""); keep(SCN_INFO4,m2,c2,0); evi_unlock(); }
void evi_caption_standby(int d){ evi_lock(); if(!need(SCN_STANDBY,0,0,d)){evi_unlock();return;} begin(); hal_move_ddram(2); hal_write_letters(" Charger Standby"); hal_move_ddram(15); big5(0xA5,0x52); big5(0xB9,0x71); big5(0xBE,0xB9); big5(0xAB,0xDD); big5(0xBE,0xF7); big5(0xA4,0xA4); if(d>0){ big5(0xA1,0x56); big5(0xA9,0xB5); big5(0xAE,0xC9);} keep(SCN_STANDBY,0,0,d); evi_unlock(); }
