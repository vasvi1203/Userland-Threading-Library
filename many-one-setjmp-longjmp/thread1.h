#include <setjmp.h>
#include <signal.h>
#define MAX_THREADS 128

typedef enum { RUNNING, READY, EXITED } STATUS;

typedef struct tcb {
    /* returned by thread create */
    int tid;  
    jmp_buf state;
    void *(*start_routine)(void *);
    void *arg;
    STATUS status;
    void *ret_val;
    sigset_t signals;
} tcb;

typedef struct spinlock{
    int islocked;
} spinlock;

extern int mutex_index;

typedef struct mutex {
    int islocked;
    spinlock spin_lock;
    //int q[MAX_THREADS];         
} mutex;

typedef long thread_t;

extern int t_index;

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_init(void);

void thread_exit(void *retval);

int thread_join(thread_t thread, void **retval);

int thread_kill(thread_t thread, int sig);

void thread_spin_init(spinlock* spin_lock);

void thread_spin_lock(spinlock* spin_lock);

void thread_spin_unlock(spinlock* spin_lock);

void thread_mutex_init(mutex *m);

void thread_mutex_block(mutex *m, spinlock *sl);

void thread_mutex_lock(mutex *m);

void thread_mutex_unlock(mutex *m);