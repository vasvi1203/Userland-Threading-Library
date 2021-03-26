#include <ucontext.h>

typedef struct tcb {
    int tid;
    ucontext_t context;
    struct tcb* parent;
    struct tcb** children;
} tcb;

typedef unsigned long thread_t;
// int pthread_create(thread_t *thread, const thread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int thread_create(thread_t *thread, void *attr, void *(*start_routine)(void *), void *arg);

void thread_exit(void *retval);
