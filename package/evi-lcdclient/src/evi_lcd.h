#ifndef EVI_LCD_H
#define EVI_LCD_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
int evi_lcd_init(void);
int evi_lcd_clear(void);
int evi_lcd_home(void);
int evi_lcd_instr(uint8_t v);
int evi_lcd_ddram(uint8_t addr);
int evi_lcd_put(const char* s);
int evi_lcd_printf(const char* fmt, ...);
void evi_lcd_set_socket(const char* path);
#ifdef __cplusplus
}
#endif
#endif
