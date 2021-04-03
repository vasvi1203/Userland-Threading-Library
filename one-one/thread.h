#define MAX_THREADS 128

typedef struct tcb {
    int tid;
    int child_tid;
    int parent_tid;
    void** ret_val;
} tcb;

tcb tcb_table[MAX_THREADS];

extern int t_index;

typedef long thread_t;
// int pthread_create(thread_t *thread, const thread_attr_t *attr, void *(*start_routine)(void *), void *arg);
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_init(void);

void thread_exit(void *retval);

int thread_join(thread_t thread);
