#define MAX_THREADS 101

// Geeks For Geeks
typedef struct tcb {
    int tid;
    int child_tid;
    void* ret_val;
} tcb;

typedef struct spinlock{
    int islocked;
} spinlock;


typedef struct mutex {
    int islocked;
    spinlock spin_lock;
    int futex_word;    
} mutex;

tcb tcb_table[MAX_THREADS];

extern int t_index;

typedef long thread_t;
// int pthread_create(thread_t *thread, const thread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_init(void);

int thread_exit(void *retval);

int thread_join(thread_t thread, void **retval);

int thread_kill(thread_t thread, int sig);

void thread_spin_init(spinlock* spin_lock);

void thread_spin_lock(spinlock* spin_lock);

void thread_spin_unlock(spinlock* spin_lock);

void thread_mutex_init(mutex *m);

void thread_mutex_block(mutex *m, spinlock *sl);

void thread_mutex_lock(mutex *m);

void thread_mutex_unlock(mutex *m);