#define MAX_THREADS 128

typedef struct tcb{
    /* returned by thread create */
    int tid;        
    int pid;
    void *(*start_routine)(void *);
    void* arg;
    void* ret_val;
}tcb;

typedef long thread_t;
extern int t_index;

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg);

void thread_init(void);

void thread_exit(void *retval);

int thread_join(thread_t thread, void **retval);

int thread_kill(thread_t thread, int sig);