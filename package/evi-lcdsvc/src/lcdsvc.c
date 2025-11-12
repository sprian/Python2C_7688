#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/file.h>
#include <fcntl.h>

#include "include/lcd_hw.h"

static const char* SOCK_PATH = "/var/run/evi-lcd.sock";
static const char* LOCK_PATH = "/var/run/evi-lcd.lock";

static int parse_hex(const char* s){ unsigned v=0; if (sscanf(s, "%x", &v)!=1) return -1; return (int)(v & 0xFF); }
static int uci_get_int(const char* key, int def){
    char cmd[256]; snprintf(cmd,sizeof(cmd),"uci -q get %s 2>/dev/null", key);
    FILE* fp=popen(cmd,"r"); if(!fp) return def;
    char buf[64]={0}; if(!fgets(buf,sizeof(buf),fp)){ pclose(fp); return def; } pclose(fp);
    int v=def; sscanf(buf,"%d",&v); return v;
}
static struct lcd_pins pins;
static int load_pins_from_uci(void){
    pins.rs = uci_get_int("lcd.@lcd[0].rs",0);
    pins.rw = uci_get_int("lcd.@lcd[0].rw",1);
    pins.en = uci_get_int("lcd.@lcd[0].en",2);
    pins.d[0]= uci_get_int("lcd.@lcd[0].d0",3);
    pins.d[1]= uci_get_int("lcd.@lcd[0].d1",4);
    pins.d[2]= uci_get_int("lcd.@lcd[0].d2",5);
    pins.d[3]= uci_get_int("lcd.@lcd[0].d3",6);
    pins.d[4]= uci_get_int("lcd.@lcd[0].d4",7);
    pins.d[5]= uci_get_int("lcd.@lcd[0].d5",8);
    pins.d[6]= uci_get_int("lcd.@lcd[0].d6",9);
    pins.d[7]= uci_get_int("lcd.@lcd[0].d7",10);
    pins.bits = 8;
    return 0;
}
static void handle_client(int cfd){
    char line[512];
    while(1){
        ssize_t n = recv(cfd, line, sizeof(line)-1, 0);
        if (n <= 0) break;
        line[n]=0;
        char* p = line;
        while(p && *p){
            char* e = strchr(p,'\n');
            if (!e) break;
            *e=0;
            if (!strncmp(p,"INIT",4)){ load_pins_from_uci(); lcd_hw_init(&pins); dprintf(cfd,"OK\n"); }
            else if (!strncmp(p,"CLEAR",5)){ lcd_clear(&pins); dprintf(cfd,"OK\n"); }
            else if (!strncmp(p,"HOME",4)){ lcd_home(&pins); dprintf(cfd,"OK\n"); }
            else if (!strncmp(p,"INSTR ",6)){ int v=parse_hex(p+6); if (v<0) dprintf(cfd,"ERR badhex\n"); else { lcd_write_instr(&pins,(unsigned char)v); dprintf(cfd,"OK\n"); } }
            else if (!strncmp(p,"PUT ",4)){ lcd_write_text(&pins,p+4); dprintf(cfd,"OK\n"); }
            else if (!strncmp(p,"DDRAM ",6)){ int v=parse_hex(p+6); if (v<0) dprintf(cfd,"ERR badhex\n"); else { lcd_set_ddram(&pins,(unsigned char)v); dprintf(cfd,"OK\n"); } }
            else { dprintf(cfd,"ERR unknown\n"); }
            p = e+1;
        }
    }
}
int main(void){
    int lfd = open(LOCK_PATH, O_CREAT|O_RDWR, 0644);
    if (lfd<0) return 1;
    if (flock(lfd, LOCK_EX|LOCK_NB)!=0) return 1;
    unlink(SOCK_PATH);
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd<0) return 1;
    struct sockaddr_un a; memset(&a,0,sizeof(a)); a.sun_family=AF_UNIX; strncpy(a.sun_path,SOCK_PATH,sizeof(a.sun_path)-1);
    if (bind(sfd,(struct sockaddr*)&a,sizeof(a))<0) return 1;
    chmod(SOCK_PATH,0666);
    if (listen(sfd,5)<0) return 1;
    load_pins_from_uci(); lcd_hw_init(&pins);
    while(1){ int cfd=accept(sfd,NULL,NULL); if (cfd<0){ usleep(10000); continue; } handle_client(cfd); close(cfd); }
    return 0;
}
