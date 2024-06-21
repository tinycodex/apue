#include "anytimer.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define JOBMAX 1024

// 函数类型
// 函数类型指针
// 函数类型指针数组
// static at_jobfunc_t *jobs[JOBMAX];
enum flag_t
{
    CREATED,
    RUNNING,
    STOPED,
    CANCELED,
};

struct job_st
{
    int sec;
    at_jobfunc_t *f;
    void *arg;
    enum flag_t flag;
    pid_t pid;
};

static struct job_st *jobs[JOBMAX];

static void alrm_handler(int signo)
{
    // printf("alarm_handler\n");
    for (size_t i = 0; i < JOBMAX; i++)
    {
        struct job_st *job = jobs[i];
        if (job == NULL)
        {
            continue;
        }

        // printf("sec %d\n",job->sec);

        if (job->flag == CREATED)
        {
            if (job->sec > 1)
            {
                --job->sec;
                continue;
            }


            job->flag = RUNNING;
            // job->f(job->arg);
            pid_t pid = fork();
            if (pid == 0)
            {
                job->f(job->arg);
                sleep(10);
                exit(0);
            }
            else if (pid < 0)
            {
                perror("fork failed()");
            }
            else
            {
                printf("child job pid:%d \n",pid);
                job->pid = pid;
            }
        }
        else if (job->flag == CANCELED || job->flag == STOPED)
        {
            printf("recycle resource %ld\n",i);
            free(job);
            jobs[i] = NULL;
        }
    }

    alarm(1);
}

static void alrm_handler2(int signo)
{
    write(1, "!", 1);
    alarm(1);
}

static int inited = 0;
static void module_init()
{
    puts("module init");
    memset(jobs, 0, sizeof(void *) * JOBMAX);
    signal(SIGALRM, alrm_handler);
    alarm(1);
}

static int find_free_pos()
{
    for (size_t i = 0; i < JOBMAX; i++)
    {
        if (jobs[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

int at_addjob(int sec, at_jobfunc_t jobp, void *arg)
{
    if (!inited)
    {
        module_init();
        inited = 1;
    }

    if (sec <= 0 || jobp == NULL)
    {
        return -EINVAL;
    }

    int pos = find_free_pos();

    if (pos < 0)
    {
        return -ENOSPC;
    }

    struct job_st *job = malloc(sizeof(struct job_st));

    if (job == NULL)
    {
        return -ENOMEM;
    }

    job->arg = arg;
    job->f = jobp;
    job->sec = sec;
    job->flag = CREATED;

    jobs[pos] = job;

    return pos;
}

int at_canceljob(int id)
{
    if (id < 0 || id >= JOBMAX)
    {
        return -EINVAL;
    }

    struct job_st *job = jobs[id];

    if (job == NULL)
    {
        return -EINVAL;
    }

    switch (job->flag)
    {
    case CANCELED:
        return -ECANCELED;
    case RUNNING:
        return -EBUSY;
    default:
        break;
    }

    job->flag = CANCELED;

    return 0;
}