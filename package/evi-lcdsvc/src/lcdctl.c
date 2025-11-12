#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

static const char* SOCK_PATH = "/var/run/evi-lcd.sock";
static int send_cmd(const char* cmd){
    int fd = socket(AF_UNIX, SOCK_STREAM, 0); if (fd<0){ perror("socket"); return 1; }
    struct sockaddr_un a; memset(&a,0,sizeof(a)); a.sun_family=AF_UNIX; strncpy(a.sun_path,SOCK_PATH,sizeof(a.sun_path)-1);
    if (connect(fd,(struct sockaddr*)&a,sizeof(a))<0){ perror("connect"); close(fd); return 1; }
    dprintf(fd, "%s\n", cmd);
    char buf[256]; int n=read(fd,buf,sizeof(buf)-1); if (n>0){ buf[n]=0; fputs(buf, stdout); }
    close(fd); return 0;
}
static void usage(const char* p){ fprintf(stderr,"Usage: %s <init|clear|home|instr 0xNN|ddram 0xNN|put TEXT>\n", p); }
int main(int argc,char**argv){
    if (argc<2){ usage(argv[0]); return 1; }
    if (!strcmp(argv[1],"init")) return send_cmd("INIT");
    if (!strcmp(argv[1],"clear")) return send_cmd("CLEAR");
    if (!strcmp(argv[1],"home")) return send_cmd("HOME");
    if (!strcmp(argv[1],"instr") && argc>=3){ char cmd[32]; snprintf(cmd,sizeof(cmd),"INSTR %s",argv[2]); return send_cmd(cmd); }
    if (!strcmp(argv[1],"ddram") && argc>=3){ char cmd[32]; snprintf(cmd,sizeof(cmd),"DDRAM %s",argv[2]); return send_cmd(cmd); }
    if (!strcmp(argv[1],"put") && argc>=3){ char buf[512]="PUT "; for (int i=2;i<argc;i++){ strncat(buf,argv[i],sizeof(buf)-strlen(buf)-1); if (i+1<argc) strncat(buf," ",sizeof(buf)-strlen(buf)-1);} return send_cmd(buf); }
    usage(argv[0]); return 1;
}
