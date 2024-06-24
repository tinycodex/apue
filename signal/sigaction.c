//
// Created by X on 2024/6/12.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_handler(int signo)
{
    printf("signo:%d begin\n", signo);
    write(1, "!", 1);
    sleep(5);
    printf("signo:%d end\n", signo);
}

void sigaction_handler(int signo, siginfo_t *infop, void *unused)
{
    printf("signo:%d si_error:%d si_code:%d si_pid:%d si_uid:%d si_status:%d si_addr:%p\n",
           infop->si_signo,
           infop->si_errno,
           infop->si_code,
           infop->si_pid,
           infop->si_uid,
           infop->si_status,
           infop->si_addr);
}

int main(int argc, char *argv[])
{
    printf("[%d]hello, world\n", getpid());
    printf("%d\n", SI_USER);

    struct sigaction sa;

    sa.sa_sigaction = sigaction_handler;
    sa.sa_flags = SA_SIGINFO;
    // sigemptyset(&sa.sa_mask);
    // sigaddset(&sa.sa_mask, SIGINT);
    // sigaddset(&sa.sa_mask, SIGTERM);
    // sigaddset(&sa.sa_mask, SIGQUIT);

    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    // sigaction(SIGTERM, &sa, NULL);
    // sigaction(SIGQUIT, &sa, NULL);

    alarm(1);

    // signal(SIGINT, sig_handler);
    // signal(SIGQUIT,sig_handler);
    // signal(SIGTERM,sig_handler);

    for (int i = 0; i < 1000; ++i)
    {
        write(1, "*", 1);
        sleep(1);
    }

    return 0;
}