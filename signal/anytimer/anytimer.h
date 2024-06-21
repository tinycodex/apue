#ifndef ANYTIMER_H
#define ANYTIMER_H

typedef void at_jobfunc_t(void*);


int at_addjob(int sec, at_jobfunc_t jobp,void *arg);

/**
 *  return  
 *          0  ok
 *          -EINVAL  参数非法
 *          -EBUSY   失败，指定任务正在运行
 *          -ECANCELLED 失败，指定任务已取消
 */
int at_canceljob(int id);


/**
 *  return 
 *          0  ok
 *          -EINVAL  参数非法
 */
int at_watijob(int id);

int at_pause(int);
int at_resume(int);


#endif