//
// Created by X on 2024/6/12.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void sig_handler(int signo){
    write(1,"!",1);
    sleep(2);
}


int main(int argc,char*argv[]){
    printf("hello, world\n");

//    signal(SIGINT, SIG_IGN);

    signal(SIGINT, sig_handler);

    for(int i = 0;i < 10;++i){
        write(1,"*",1);
        sleep(1);
    }

    return 0;
}