#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
int procctl_freeze() {
    FILE* f = fopen("/tmp/mainPid","r"); if(!f) return -1;
    char buf[32]={0}; if(!fgets(buf,sizeof(buf),f)){ fclose(f); return -1; }
    fclose(f); int pid = atoi(buf); if(pid<=0) return -1; return kill(pid, SIGSTOP);
}
int procctl_thaw() {
    FILE* f = fopen("/tmp/mainPid","r"); if(!f) return -1;
    char buf[32]={0}; if(!fgets(buf,sizeof(buf),f)){ fclose(f); return -1; }
    fclose(f); int pid = atoi(buf); if(pid<=0) return -1; return kill(pid, SIGCONT);
}
