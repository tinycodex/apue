#include "anytimer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void f1(void *p)
{
    printf("pid:%d %s\n", getpid(), (char *)p);
}

int main(int argc, char const *argv[])
{
    int retval, job1, job2, job3;
    if ((retval = at_addjob(1, NULL, "111")) < 0)
    {
        printf("%s\n", strerror(-retval));
    }

    puts("Beging!");
    job1 = at_addjob(5, f1, "aaa");
    job2 = at_addjob(2, f1, "bbb");
    job3 = at_addjob(3, f1, "ccc");
    job3 = at_addjob(7, f1, "ddd");
    puts("End!");

    while (1)
    {
        write(1, ".", 1);
        sleep(1);
    }

    return 0;
}
