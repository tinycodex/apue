#ifndef MYTBF_H_
#define MYTBF_H_

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps, int burst);

int mytbf_fetchtoken(mytbf_t *ptr, int n);

int mytbf_returntoken(mytbf_t *ptr, int n);

int mytbf_destroy(mytbf_t *);




#endif