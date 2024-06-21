#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

void alrm_handler(int signo)
{
    printf("%d pid:%d\n", signo, getpid());
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("parent:%d\n", getpid());
        wait(NULL);
    }
    else if (pid == 0)
    {
        printf("child:%d\n", getpid());
    }
    else
    {
        perror("fork() failed\n");
    }
}

static void exit_hook(){
    printf("exit_hook:%d\n",getpid());
}

int main(int argc, char const *argv[])
{
    signal(SIGALRM, alrm_handler);
    alarm(1);
    atexit(exit_hook);

    // alrm_handler(1);
    while (1)
    {
        printf("pid:%d\n", getpid());
        write(1, ".", 1);
        sleep(1);
        printf("---end pid:%d\n", getpid());
    }

    return 0;
}
