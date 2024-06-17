#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "mytbf.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
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

    mytbf_t *tbf = mytbf_init(50, 100);

    char buf[BUFSIZE];
    int n;

    while (1)
    {

        int tokenCount = mytbf_fetchtoken(tbf, BUFSIZ);

        // printf("applied tokenCount:%d\n",tokenCount);

        while ((n = read(sfd, buf, tokenCount)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("read");
            exit(errno);
        }

        if (n == 0)
        {
            break;
        }

        if (tokenCount - n > 0)
        {
            mytbf_returntoken(tbf, tokenCount - n);
        }

        int wn = 0;

        while (wn < n)
        {
            int m = write(STDOUT_FILENO, buf + wn, n - wn);

            if (m < 0)
            {
                if (errno == EINTR)
                {
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
