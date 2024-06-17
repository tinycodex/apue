//
// Created by X on 2024/6/13.
//

#include<stdio.h>
#include <unistd.h>
#include <signal.h>

static  int loop = 1;

void alarm_handler(int signo){
    loop = 0;
    printf("alarm_no:%d\n",signo);
}

int main(int argc, char* argv[]){
    long long count = 0;

    alarm(5);

    signal(SIGALRM, alarm_handler);

    while(loop){
        count ++;
    }


    printf("count = %lld\n",count);
}