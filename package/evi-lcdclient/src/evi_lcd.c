#define _GNU_SOURCE
#include "evi_lcd.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

static char g_sock_path[128] = "/var/run/evi-lcd.sock";
static pthread_mutex_t g_mu = PTHREAD_MUTEX_INITIALIZER;

void evi_lcd_set_socket(const char* path){
    if (!path) return;
    pthread_mutex_lock(&g_mu);
    strncpy(g_sock_path, path, sizeof(g_sock_path)-1);
    g_sock_path[sizeof(g_sock_path)-1] = 0;
    pthread_mutex_unlock(&g_mu);
}

static int send_cmd(const char* cmd){
    int rc = -1;
    pthread_mutex_lock(&g_mu);
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0){ rc = -2; goto out; }
    struct sockaddr_un addr; memset(&addr,0,sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, g_sock_path, sizeof(addr.sun_path)-1);
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr))<0){ rc = -3; goto out_close; }
    dprintf(fd, "%s\n", cmd);
    char buf[256]; int n = read(fd, buf, sizeof(buf)-1);
    if (n <= 0){ rc = -4; goto out_close; }
    buf[n] = 0;
    rc = (strncmp(buf, "OK", 2)==0) ? 0 : -5;
out_close:
    close(fd);
out:
    pthread_mutex_unlock(&g_mu);
    return rc;
}

int evi_lcd_init(void){ return send_cmd("INIT"); }
int evi_lcd_clear(void){ return send_cmd("CLEAR"); }
int evi_lcd_home(void){ return send_cmd("HOME"); }
int evi_lcd_instr(uint8_t v){ char cmd[32]; snprintf(cmd, sizeof(cmd), "INSTR 0x%02X", v); return send_cmd(cmd); }
int evi_lcd_ddram(uint8_t addr){ char cmd[32]; snprintf(cmd, sizeof(cmd), "DDRAM 0x%02X", addr); return send_cmd(cmd); }
int evi_lcd_put(const char* s){ char buf[600]; snprintf(buf, sizeof(buf), "PUT %s", s ? s : ""); return send_cmd(buf); }
int evi_lcd_printf(const char* fmt, ...){
    char text[512]; va_list ap; va_start(ap, fmt); vsnprintf(text, sizeof(text), fmt, ap); va_end(ap);
    char buf[600]; snprintf(buf, sizeof(buf), "PUT %s", text); return send_cmd(buf);
}
