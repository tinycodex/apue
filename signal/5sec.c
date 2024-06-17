//
// Created by X on 2024/6/13.
//

#include<stdio.h>
#include <time.h>


int main(int argc, char* argv[]){
    long long count = 0;

    time_t deadline = time(NULL) + 5;


    while(time(NULL) <= deadline){
        count ++;
    }

    printf("count = %lld\n",count);
}