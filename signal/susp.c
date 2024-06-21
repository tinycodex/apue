#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sig_quit_handler(int signo)
{
    write(1, "!", 1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_quit_handler);

    sigset_t sigset, oset;

    sigemptyset(&sigset);
    sigemptyset(&oset);
    sigaddset(&sigset, SIGINT);

    // 这个语句的目的是设置一个运行时基准，并将旧的保存，退出时恢复。
    sigprocmask(SIG_UNBLOCK, &sigset, &oset);

    sigset_t tmpset;
    sigprocmask(SIG_BLOCK, &sigset, &tmpset);
    while (1)
    {

        // sigset_t tmpset;
        // sigprocmask(SIG_BLOCK,&sigset,&tmpset);
        for (int i = 0; i < 5; ++i)
        {
            write(1, ".", 1);
            sleep(1);
        }
        write(1, "\n", 1);

        // sigprocmask(SIG_SETMASK,&tmpset,NULL);
        // pause();
        sigsuspend(&tmpset);
    }

    sigprocmask(SIG_SETMASK, &oset, NULL);

    exit(0);
}
