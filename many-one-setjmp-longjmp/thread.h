#ifndef THREAD_H
#define THREAD_H

#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#define MAX_THREADS 128

// @credit:- https://sites.cs.ucsb.edu/~chris/teaching/cs170/projects/proj2.html
typedef enum { RUNNING, READY, EXITED, WAITING } STATUS;

typedef struct tcb {
    /* returned by thread create */
    int tid;  
    jmp_buf state;
    void *(*start_routine)(void *);
    void *arg;
    STATUS status;
    void *ret_val;
    sigset_t signals;
    void *stack;
} tcb;

typedef struct node {
    tcb* t;
    struct node* next;
} node;

typedef struct queue {
    node *head, *tail;
    int size;
} queue;

typedef struct spinlock {
    int islocked;
} spinlock;

extern int mutex_index;

typedef struct mutex {
    int islocked;
    spinlock spin_lock;
    queue wait_queue;         
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

void initQ(queue *q);

void enQ(queue *q, tcb *t);

tcb* deQ(queue *q);

tcb* search_thread(queue* q, thread_t thread);

void remove_thread(queue* q, thread_t thread);

#endif
