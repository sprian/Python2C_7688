#include "../include/evi_hal.h"
#include "evi_charge.h"


void evi_charge_preparing(void){
  hal_move_ddram(2);
  hal_write_letters("Charger Preparing");
  hal_move_ddram(15);
  hal_write_graphic(0xA5);
  hal_write_graphic(0x52);
  hal_delay_us(72);
  hal_write_graphic(0xB9);
  hal_write_graphic(0x71);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0xB9);
  hal_delay_us(72);
  hal_write_graphic(0xB7);
  hal_write_graphic(0xC7);
  hal_delay_us(72);
  hal_write_graphic(0xB3);
  hal_write_graphic(0xC6);
  hal_delay_us(72);
  hal_write_graphic(0xA4);
  hal_write_graphic(0xA4);
  hal_delay_us(72);
}

void evi_charge_auth_wait_char(void){
  hal_move_ddram(3);
  hal_write_graphic(0xB5);
  hal_write_graphic(0xA5);
  hal_delay_us(72);
  hal_write_graphic(0xAB);
  hal_write_graphic(0xDD);
  hal_delay_us(72);
  hal_write_graphic(0xA8);
  hal_write_graphic(0xAE);
  hal_delay_us(72);
  hal_write_graphic(0xBA);
  hal_write_graphic(0xDD);
  hal_delay_us(72);
  hal_write_graphic(0xB3);
  hal_write_graphic(0x73);
  hal_delay_us(72);
  hal_write_graphic(0xB1);
  hal_write_graphic(0xB5);
  hal_delay_us(72);
  hal_move_ddram(14);
  hal_write_graphic(0xB5);
  hal_write_graphic(0xA5);
  hal_delay_us(72);
  hal_write_graphic(0xAB);
  hal_write_graphic(0xDD);
  hal_delay_us(72);
  hal_write_graphic(0xAE);
  hal_write_graphic(0xC9);
  hal_delay_us(72);
  hal_write_graphic(0xB6);
  hal_write_graphic(0xA1);
  hal_delay_us(72);
  hal_write_graphic(0xA9);
  hal_write_graphic(0x7C);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0x6C);
  hal_delay_us(72);
  hal_move_ddram(20);
  hal_move_ddram(22);
  hal_write_graphic(0xAC);
  hal_write_graphic(0xED);
  hal_delay_us(72);
}

void evi_charge_auth_wait_eng(void){
  hal_move_ddram(0);
  hal_write_letters("Wait Connect. to The Car");
  hal_move_ddram(13);
}

void evi_charge_plugin_wait_char(void){
  hal_move_ddram(3);
  hal_write_graphic(0xB5);
  hal_write_graphic(0xA5);
  hal_delay_us(72);
  hal_write_graphic(0xAB);
  hal_write_graphic(0xDD);
  hal_delay_us(72);
  hal_write_graphic(0xA5);
  hal_write_graphic(0x52);
  hal_delay_us(72);
  hal_write_graphic(0xB9);
  hal_write_graphic(0x71);
  hal_delay_us(72);
  hal_write_graphic(0xB1);
  hal_write_graphic(0xD2);
  hal_delay_us(72);
  hal_write_graphic(0xB0);
  hal_write_graphic(0xCA);
  hal_delay_us(72);
  hal_move_ddram(14);
  hal_write_graphic(0xB5);
  hal_write_graphic(0xA5);
  hal_delay_us(72);
  hal_write_graphic(0xAB);
  hal_write_graphic(0xDD);
  hal_delay_us(72);
  hal_write_graphic(0xAE);
  hal_write_graphic(0xC9);
  hal_delay_us(72);
  hal_write_graphic(0xB6);
  hal_write_graphic(0xA1);
  hal_delay_us(72);
  hal_write_graphic(0xA9);
  hal_write_graphic(0x7C);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0x6C);
  hal_delay_us(72);
  hal_move_ddram(20);
  hal_move_ddram(22);
  hal_write_graphic(0xAC);
  hal_write_graphic(0xED);
  hal_delay_us(72);
}

void evi_charge_plugin_wait_eng(void){
  hal_move_ddram(1);
  hal_write_letters(" Wait Starting-up");
  hal_move_ddram(13);
}

void evi_charge_get_mvalue(void){
}

void evi_charge_disp_complete(void){
}

void evi_charge_server_conn(void){
  hal_move_ddram(0);
  hal_write_letters("Connecting to the Server");
  hal_move_ddram(15);
  hal_write_graphic(0xA6);
  hal_write_graphic(0xF8);
  hal_delay_us(72);
  hal_write_graphic(0xAA);
  hal_write_graphic(0x41);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0xB9);
  hal_delay_us(72);
  hal_write_graphic(0xB3);
  hal_write_graphic(0x73);
  hal_delay_us(72);
  hal_write_graphic(0xBD);
  hal_write_graphic(0x75);
  hal_delay_us(72);
  hal_write_graphic(0xA4);
  hal_write_graphic(0xA4);
  hal_delay_us(72);
}

void evi_charge_server_conn_failed(void){
  hal_move_ddram(2);
  hal_write_letters("Connection Failed");
  hal_move_ddram(14);
  hal_write_graphic(0xA1);
  hal_write_graphic(0x40);
  hal_delay_us(72);
  hal_write_graphic(0xA6);
  hal_write_graphic(0xF8);
  hal_delay_us(72);
  hal_write_graphic(0xAA);
  hal_write_graphic(0x41);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0xB9);
  hal_delay_us(72);
  hal_write_graphic(0xB3);
  hal_write_graphic(0x73);
  hal_delay_us(72);
  hal_write_graphic(0xBD);
  hal_write_graphic(0x75);
  hal_delay_us(72);
  hal_write_graphic(0xA5);
  hal_write_graphic(0xA2);
  hal_delay_us(72);
  hal_write_graphic(0xB1);
  hal_write_graphic(0xD1);
  hal_delay_us(72);
}

void evi_charge_server_conn_lost(void){
  hal_move_ddram(2);
  hal_write_letters("Connection Lost");
  hal_move_ddram(14);
  hal_write_graphic(0xBB);
  hal_write_graphic(0x50);
  hal_delay_us(72);
  hal_write_graphic(0xA6);
  hal_write_graphic(0xF8);
  hal_delay_us(72);
  hal_write_graphic(0xAA);
  hal_write_graphic(0x41);
  hal_delay_us(72);
  hal_write_graphic(0xBE);
  hal_write_graphic(0xB9);
  hal_delay_us(72);
  hal_write_graphic(0xA5);
  hal_write_graphic(0xA2);
  hal_delay_us(72);
  hal_write_graphic(0xA5);
  hal_write_graphic(0x68);
  hal_delay_us(72);
  hal_write_graphic(0xB3);
  hal_write_graphic(0x73);
  hal_delay_us(72);
  hal_write_graphic(0xBD);
  hal_write_graphic(0x75);
  hal_delay_us(72);
}