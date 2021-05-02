#ifndef LOCK_H
#define LOCK_H 

#include "queue.h"

typedef struct spinlock{
    int islocked;
} spinlock;


typedef struct mutex {
    int islocked;
    spinlock spin_lock;
    queue wait_queue;
} mutex;

void thread_spin_init(spinlock*);

void thread_spin_lock(spinlock*);

void thread_spin_unlock(spinlock*);

void thread_mutex_init(mutex*);

void thread_mutex_lock(mutex*);

void thread_mutex_unlock(mutex*);

#endif /* LOCK_H */