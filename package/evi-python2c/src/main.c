#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
static volatile int running = 1;
static void on_sig(int s){ (void)s; running = 0; }
int main(void){
    signal(SIGINT,on_sig); signal(SIGTERM,on_sig);
    int ep = epoll_create1(0);
    if (ep<0){ perror("epoll_create1"); return 1; }
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
    if (tfd<0){ perror("timerfd_create"); return 2; }
    struct itimerspec its={{1,0},{1,0}};
    if (timerfd_settime(tfd,0,&its,NULL)<0){ perror("timerfd_settime"); return 3; }
    struct epoll_event ev; memset(&ev,0,sizeof(ev)); ev.events=EPOLLIN; ev.data.fd=tfd;
    if (epoll_ctl(ep,EPOLL_CTL_ADD,tfd,&ev)<0){ perror("epoll_ctl"); return 4; }
    fprintf(stderr,"evi-python2c v0.9.1 running (tick=1s)\n");
    while(running){
        int n = epoll_wait(ep,&ev,1,-1);
        if (n<0){ if(errno==EINTR) continue; perror("epoll_wait"); break; }
        if (ev.data.fd==tfd && (ev.events & EPOLLIN)){
            uint64_t x; if (read(tfd,&x,sizeof(x))<=0) continue;
            fprintf(stderr,"[tick] %llu\n",(unsigned long long)x);
        }
    }
    close(tfd); close(ep);
    fprintf(stderr,"bye\n");
    return 0;
}
