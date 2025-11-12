\
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCK_PATH "/var/run/azureiotd.sock"
static int send_cmd(const char* cmd){
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd<0) return -1;
    struct sockaddr_un sa; memset(&sa,0,sizeof(sa));
    sa.sun_family = AF_UNIX; strncpy(sa.sun_path, SOCK_PATH, sizeof(sa.sun_path)-1);
    if(connect(fd,(struct sockaddr*)&sa,sizeof(sa))<0){ close(fd); return -2; }
    write(fd, cmd, strlen(cmd)); write(fd, "\n", 1);
    char buf[128]; int n = read(fd, buf, sizeof(buf)-1);
    close(fd);
    if(n<=0) return -3; buf[n]=0;
    return (strncmp(buf,"OK",2)==0)?0:-4;
}
int evi_azure_send_json(const char* json){
    if(!json) return -1;
    char line[4096]; snprintf(line,sizeof(line),"SEND %s",json);
    return send_cmd(line);
}
int evi_azure_status(char* out,int outlen){
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd<0) return -1;
    struct sockaddr_un sa; memset(&sa,0,sizeof(sa));
    sa.sun_family = AF_UNIX; strncpy(sa.sun_path, SOCK_PATH, sizeof(sa.sun_path)-1);
    if(connect(fd,(struct sockaddr*)&sa,sizeof(sa))<0){ close(fd); return -2; }
    write(fd,"STATUS\n",7);
    int n = read(fd,out,outlen>0?outlen:0);
    close(fd);
    if(n<=0) return -3;
    if(out && outlen>0){ if(n>=outlen) n=outlen-1; out[n]=0; }
    return 0;
}
