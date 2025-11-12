#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "include/lcd_hw.h"
#include "include/gpio_sysfs.h"

struct lcd_ctx { struct lcd_pins pins; int fd_rs, fd_rw, fd_en; int fd_d[8]; } g;
void lcd_delay_us(int us){ usleep(us); }
static int set_data_bus(int val){ for (int i=0;i<8;i++){ if (g.fd_d[i]>=0 && gpio_write_fd(g.fd_d[i], (val>>i)&1)!=0) return -1; } return 0; }
static int pulse_en(){ if (gpio_write_fd(g.fd_en,1)!=0) return -1; lcd_delay_us(1); if (gpio_write_fd(g.fd_en,0)!=0) return -1; lcd_delay_us(50); return 0; }
int lcd_hw_init(struct lcd_pins* p){
    memset(&g, 0, sizeof(g)); g.pins=*p;
    int pins[11] = {p->rs,p->rw,p->en,p->d[0],p->d[1],p->d[2],p->d[3],p->d[4],p->d[5],p->d[6],p->d[7]};
    for (int i=0;i<11;i++) gpio_export(pins[i]); usleep(10000);
    gpio_direction(p->rs,1); gpio_direction(p->rw,1); gpio_direction(p->en,1);
    for (int i=0;i<8;i++) gpio_direction(p->d[i],1);
    g.fd_rs=gpio_open_handle(p->rs,1); g.fd_rw=gpio_open_handle(p->rw,1); g.fd_en=gpio_open_handle(p->en,1);
    for (int i=0;i<8;i++) g.fd_d[i]=gpio_open_handle(p->d[i],1);
    gpio_write_fd(g.fd_rs,0); gpio_write_fd(g.fd_rw,0); gpio_write_fd(g.fd_en,0);
    for (int i=0;i<8;i++) gpio_write_fd(g.fd_d[i],0);
    usleep(15000);
    // Function Set: 8-bit, 1-line (legacy initLCM.py behavior)
    // Power-on delay already done above (usleep(15000))
    lcd_write_instr(p, 0x30);
    usleep(4500);
    lcd_write_instr(p, 0x30);
    usleep(150);
    // display off
    lcd_write_instr(p, 0x08);
    // clear
    lcd_clear(p);
    // entry mode
    lcd_write_instr(p, 0x06);
    // display on
    lcd_write_instr(p, 0x0C);
    return 0;
}
void lcd_hw_free(void){ gpio_close_fd(g.fd_rs); gpio_close_fd(g.fd_rw); gpio_close_fd(g.fd_en); for (int i=0;i<8;i++) gpio_close_fd(g.fd_d[i]); }
int lcd_write_instr(struct lcd_pins* p, uint8_t v){ (void)p; gpio_write_fd(g.fd_rs,0); gpio_write_fd(g.fd_rw,0); set_data_bus(v); return pulse_en(); }
int lcd_write_data(struct lcd_pins* p, uint8_t v){ (void)p; gpio_write_fd(g.fd_rs,1); gpio_write_fd(g.fd_rw,0); set_data_bus(v); return pulse_en(); }
int lcd_write_text(struct lcd_pins* p, const char* s){ for (const unsigned char* u=(const unsigned char*)s; *u; ++u){ lcd_write_data(p,*u); lcd_delay_us(40);} return 0; }
int lcd_set_ddram(struct lcd_pins* p, uint8_t addr){ return lcd_write_instr(p, 0x80 | (addr & 0x7F)); }
int lcd_clear(struct lcd_pins* p){ int rc=lcd_write_instr(p,0x01); usleep(2000); return rc; }
int lcd_home(struct lcd_pins* p){ int rc=lcd_write_instr(p,0x02); usleep(2000); return rc; }
