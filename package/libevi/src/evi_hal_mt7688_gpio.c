#include "../include/evi_hal.h"
#include "evi_hal_gpio_conf.h"
#include "gpio_sysfs.h"
#include <unistd.h>

static int inited = 0;
static void set_n(int n){ gpio_write(EVI_GPIO_D4,(n>>0)&1); gpio_write(EVI_GPIO_D5,(n>>1)&1); gpio_write(EVI_GPIO_D6,(n>>2)&1); gpio_write(EVI_GPIO_D7,(n>>3)&1); }
static void pulse(){ gpio_write(EVI_GPIO_EN,1); usleep(EVI_US_PULSE); gpio_write(EVI_GPIO_EN,0); usleep(EVI_US_PULSE); }
static void w4(int n,int rs){
  gpio_write(EVI_GPIO_RS,rs?1:0);
#if EVI_GPIO_RW >= 0
  gpio_write(EVI_GPIO_RW,0);
#endif
  set_n(n&0xF); pulse(); usleep(EVI_US_SETTLE);
}
static void w8(unsigned char v,int rs){ w4((v>>4)&0xF,rs); w4(v&0xF,rs); }
static void ensure(){
  if(inited) return;
  gpio_export(EVI_GPIO_RS); gpio_export(EVI_GPIO_EN); gpio_export(EVI_GPIO_D4);
  gpio_export(EVI_GPIO_D5); gpio_export(EVI_GPIO_D6); gpio_export(EVI_GPIO_D7);
  gpio_set_dir(EVI_GPIO_RS,1); gpio_set_dir(EVI_GPIO_EN,1); gpio_set_dir(EVI_GPIO_D4,1);
  gpio_set_dir(EVI_GPIO_D5,1); gpio_set_dir(EVI_GPIO_D6,1); gpio_set_dir(EVI_GPIO_D7,1);
  usleep(15000);
  w4(0x03,0); usleep(4500); w4(0x03,0); usleep(150); w4(0x03,0); usleep(150);
  w4(0x02,0); usleep(150);
  w8(0x28,0); w8(0x08,0); w8(0x01,0); usleep(1600); w8(0x06,0); w8(0x0C,0);
  inited = 1;
}
void hal_write_instr(unsigned char c){ ensure(); w8(c,0); }
void hal_write_letters(const char* s){ ensure(); if(!s) return; const unsigned char* p=(const unsigned char*)s; while(*p){ w8(*p++,1);} }
void hal_write_graphic(unsigned char c){ ensure(); w8(c,1); }
void hal_delay_us(unsigned int us){ usleep(us); }
void hal_move_ddram(unsigned char a){ hal_write_instr(0x80u | (a & 0x7Fu)); }
