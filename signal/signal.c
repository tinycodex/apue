//
// Created by X on 2024/6/12.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_handler(int signo){
    printf("signo:%d begin\n",signo);
    write(1,"!",1);
    sleep(5);
    printf("signo:%d end\n",signo);
}


int main(int argc,char*argv[]){
    printf("[%d]hello, world\n",getpid());


//    signal(SIGINT, SIG_IGN);

    signal(SIGINT, sig_handler);
    signal(SIGQUIT,sig_handler);
    signal(SIGTERM,sig_handler);
    

    for(int i = 0;i < 1000;++i){
        write(1,"*",1);
        sleep(1);
    }

    return 0;
}