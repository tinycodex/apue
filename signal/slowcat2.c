#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define BUFSIZE 50

static int token = 0;

void alrm_handler(int signo){
    // printf("token:%d\n",token);
    token++;
    alarm(1);
}


int main(int argc, char const *argv[])
{
    if (argc < 2){
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int sfd;
    sfd = open(argv[1], O_RDONLY);

    if (sfd < 0)
    {
        perror("open");
        return 1;
    }

    char buf[BUFSIZE];
    int n;

    signal(SIGALRM,alrm_handler);
    alarm(1);

    while(1) {
        while(token <= 0){
            pause();
        }

        token -= 1;

        while ((n = read(sfd, buf, BUFSIZE)) < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("read");
            break;
        }

        if (n == 0) {
            break;
        }


        int wn = 0;

        while (wn < n) {
            int m = write(STDOUT_FILENO,buf+wn,n-wn);

            if(m < 0){
                if(errno == EINTR){
                    continue;
                }
                perror("write failed()");
                exit(errno);
            }

            wn += m;
        }
    }

    return 0;
}
