#define MAX_THREADS 128

// Geeks For Geeks
typedef struct tcb {
    int tid;
    int child_tid;
    int parent_tid;
    void* ret_val;
} tcb;

typedef struct spinlock{
    int islocked;
} spinlock;

//typedef int thread_spinlock_t;

extern int mutex_index;

typedef struct mutex {
    int islocked;
    spinlock spin_lock;
    int q[MAX_THREADS];         
} mutex;

tcb tcb_table[MAX_THREADS];

extern int t_index;

typedef long thread_t;
// int pthread_create(thread_t *thread, const thread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_init(void);

void thread_exit(void *retval);

int thread_join(thread_t thread, void **retval);

int thread_kill(thread_t thread, int sig);

void init_spin_lock(spinlock* spin_lock);

void acquire_spin_lock(spinlock* spin_lock);

void release_spin_lock(spinlock* spin_lock);

void init_mutex(mutex *m);

void block(mutex *m, spinlock *sl);

void release(mutex *m);

void acquire(mutex *m);