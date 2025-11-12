#ifndef LCD_PROTO_H
#define LCD_PROTO_H
// Simple line protocol over UNIX socket: one command per line.
//
// Commands:
// INIT
// CLEAR
// HOME
// INSTR 0xNN
// PUT <utf8 text>
// DDRAM 0xNN
//
// Replies: "OK\n" or "ERR <msg>\n"
#endif
