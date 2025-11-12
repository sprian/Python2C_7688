#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void hal_write_instr(uint8_t code);
void hal_write_letters(const char* ascii_utf8);
void hal_write_graphic(uint8_t code);
void hal_delay_us(unsigned int us);
void hal_move_ddram(uint8_t addr);
#ifdef __cplusplus
}
#endif
