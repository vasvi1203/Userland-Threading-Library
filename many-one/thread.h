#ifndef THREAD_H
#define THREAD_H

#include<ucontext.h>
#define STACK 4096
#define MAX_THREADS 128

typedef struct tcb{ 
    /* returned by thread create */
    int tid;        
    int index;
    ucontext_t context;
    void *(*start_routine)(void *);
    void* arg;
    void* ret_val;
    short completed;
    sigset_t signals;
}tcb;

typedef long thread_t;
extern int t_index;

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void init_threads(void);

void thread_exit(void *retval);

int thread_join(thread_t thread, void **retval);

int thread_kill(thread_t thread, int sig);

#endif /* THREAD_H */