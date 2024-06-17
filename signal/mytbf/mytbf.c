#include "mytbf.h"

#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct mytbf_st
{
    int cps;
    int burst;
    int token;
    int pos;
};

#ifndef JOBSIZE
#define JOBSIZE 1024
#endif

static struct mytbf_st *jobs[JOBSIZE];

typedef void (*sig_hanlder_t)(int);

static sig_hanlder_t alrm_handler_old;

static void alrm_handler(int signo)
{
    for (size_t i = 0; i < JOBSIZE; i++)
    {
        struct mytbf_st *cur = jobs[i];
        if (cur != NULL)
        {
            cur->token += cur->cps;

            if (cur->token > cur->burst)
            {
                cur->token = cur->burst;
            }
        }
    }

    alarm(1);
}

static void module_unload()
{
    signal(SIGALRM, alrm_handler_old);

    for (size_t i = 0; i < JOBSIZE; i++)
    {
        if (jobs[i] != NULL)
        {
            free(jobs[i]);
            jobs[i] = NULL;
        }
    }
}

static int inited = 0;

static void module_load()
{
    memset(jobs, 0, sizeof(struct mytbf_t *) * JOBSIZE);

    alrm_handler_old = signal(SIGALRM, alrm_handler);
    alarm(1);

    atexit(module_unload);
}

static int find_free_pos()
{
    for (int i = 0; i < JOBSIZE; ++i)
    {
        if (jobs[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

mytbf_t *mytbf_init(int cps, int burst)
{
    if (!inited)
    {
        inited = 1;
        module_load();
    }

    int pos = find_free_pos();

    if (pos < 0)
    {
        return NULL;
    }

    struct mytbf_st *me = malloc(sizeof(struct mytbf_st));

    if (me == NULL)
    {
        return NULL;
    }

    me->pos = pos;
    me->burst = burst;
    me->cps = cps;
    me->token = 0;
    jobs[pos] = me;

    return me;
}

static int  min(int a, int b)
{
    return a < b ? a : b;
}

int mytbf_fetchtoken(mytbf_t *ptr, int n)
{
    struct mytbf_st *me = ptr;

    // if (n > me->burst)
    // {
    //     return -EINVAL;
    // }

    // printf("token:%d\n",me->token);
    
    while (me->token <= 0)
    {
        pause();
    }


    // printf("token:%d,n:%d\n",me->token,n);

    int token = min(me->token, n);

    me->token -= token;

    // printf("token:%d,n:%d\n",me->token,n);
    return token;
}

int mytbf_returntoken(mytbf_t *ptr, int n)
{
    struct mytbf_st *me = ptr;
    if (n < 0)
    {
        return -EINVAL;
    }

    me->token += n;

    if (me->token > me->burst)
    {
        me->token = me->burst;
    }

    return n;
}

int mytbf_destroy(mytbf_t *ptr)
{
    struct mytbf_st *me = ptr;

    jobs[me->pos] = NULL;
    free(me);

    return 0;
}
