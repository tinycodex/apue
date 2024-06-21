#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

void alrm_handler(int signo)
{
    printf("%d pid:%d\n", signo,getpid());
}

int main(int argc, char const *argv[])
{
    signal(SIGALRM,alrm_handler);
    alarm(1);

    pid_t pid = fork();

    if (pid == 0)
    {
        sleep(5);
        printf("pid %d, ppid:%d\n", getpid(), getppid());
    }
    else if (pid > 0)
    {
        sleep(5);
        printf("child pid:%d pid:%d ppid:%d\n", pid, getpid(), getppid());
        wait(NULL);
    }
    else
    {
        printf("wrong %s", strerror(errno));
    }

    return 0;
}
