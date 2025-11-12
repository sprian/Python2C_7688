#ifndef LCD_HW_H
#define LCD_HW_H
#include <stdint.h>
struct lcd_pins { int rs, rw, en; int d[8]; int bits; };
int lcd_hw_init(struct lcd_pins* p);
void lcd_hw_free(void);
void lcd_delay_us(int us);
int lcd_write_instr(struct lcd_pins* p, uint8_t v);
int lcd_write_data(struct lcd_pins* p, uint8_t v);
int lcd_write_text(struct lcd_pins* p, const char* s);
int lcd_set_ddram(struct lcd_pins* p, uint8_t addr);
int lcd_clear(struct lcd_pins* p);
int lcd_home(struct lcd_pins* p);
#endif
