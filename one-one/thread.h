
#define _GNU_SOURCE
#include <sched.h>
#include <ucontext.h>
typedef struct tcb {
    int tid;
    pid_t child_tid;
    pid_t parent_tid;
    void* ret_val;
} tcb;

pid_t cid;

typedef unsigned long thread_t;
// int pthread_create(thread_t *thread, const thread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_exit(void *retval);
