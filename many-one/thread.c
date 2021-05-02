#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<errno.h>
#include "thread.h"
#include "queue.h"

int t_index = 0;
int no_of_threads = 0;
ucontext_t signal_context;          /* the interrupt context */
void *signal_stack;                 /* global interrupt stack */

queue* ready_queue;
queue* finished_queue;
tcb* current_thread;

void disable_timer(void){
    sigset_t sigvtalrm;
    sigemptyset(&sigvtalrm);
    sigaddset(&sigvtalrm, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &sigvtalrm, NULL);
}

void enable_timer(){
    sigset_t sigvtalrm;
    sigemptyset(&sigvtalrm);
    sigaddset(&sigvtalrm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &sigvtalrm, NULL);
}


void schedule()
{
    disable_timer();
    
    if(current_thread->completed != 2){
        enQ(ready_queue,current_thread);
    }
    
    current_thread = deQ(ready_queue);
    for(int i = 1; i < 32; i++) {
        if(sigismember(&current_thread->signals, i)) {
            raise(i);
            sigdelset(&current_thread->signals,i);
        }
    }
    // printf("id:- %d\n",current_thread->index);
    enable_timer();
    setcontext(&current_thread->context);

}

/*
  Timer interrupt handler.
  Creates a new context to run the scheduler in, masks signals, then swaps
  contexts saving the previously executing thread and jumping to the
  scheduler.
*/
void timer_interrupt_handler(int i, siginfo_t *siginfo, void *old_context)
{
    /* Create new scheduler context */
    getcontext(&signal_context);
    signal_context.uc_stack.ss_sp = signal_stack;
    signal_context.uc_stack.ss_size = STACK;
    signal_context.uc_stack.ss_flags = 0;
    sigemptyset(&signal_context.uc_sigmask);
    makecontext(&signal_context, schedule, 1);

    /* save running thread, jump to scheduler */
    swapcontext(&current_thread->context,&signal_context);
}

void setup_timer(){
    // array representing signals
    sigset_t signal_set;
    // set all entries in the array
    sigfillset(&signal_set);

    struct sigaction ticker;
    // ticker.sa_sigaction = scheduler;  // signal handler
    ticker.sa_sigaction = timer_interrupt_handler;  // signal handler
    ticker.sa_mask = signal_set;      
    ticker.sa_flags =  0;             // don't use SA_RESTARAT

    // SIGVTALRM suits more than SIGALRM
    // handle SIGVTALRM with ticker configurations
    sigaction(SIGVTALRM,&ticker,NULL);

    struct itimerval tick;
    // //  /* when timer expires */
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 1000;
    // // /* Time to the next timer expiration. */
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 1;

    setitimer(ITIMER_VIRTUAL,&tick,NULL);
    // printf("TIMER INIT\n");
}

void make_main_thread_context(){
    tcb* main_thread = (tcb *)malloc(sizeof(tcb));
    main_thread->tid = getpid();
    main_thread->index = t_index ++;
    main_thread->completed = 0;
    sigemptyset(&main_thread->signals);
    no_of_threads++;
    getcontext(&main_thread->context);
    current_thread = main_thread;
}

void init_threads(){
    ready_queue = (queue *)malloc(sizeof(queue));
    finished_queue = (queue *)malloc(sizeof(queue));
    initQ(ready_queue);
    initQ(finished_queue);
    setup_timer();
    signal_stack = malloc(STACK);
    // kernal scheduler is still running the main thread
    // on timer we change the context to run another thread
    make_main_thread_context();
}

void wrap_start_routine(){
    disable_timer();
    tcb* run = current_thread;
    enable_timer();
    void* result = run->start_routine(run->arg);
    thread_exit(result);
}

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg){
    disable_timer();
    // create context for current thread
    // put it ready q
    if(no_of_threads >= 128){
        printf("Thread can't be created\n");
        return 1;
    }
    if(start_routine == NULL){
        perror("Invalid start routine!\n");
        return EINVAL;
    }
    tcb* this = (struct tcb *)malloc(sizeof(struct tcb));
    this->tid = getpid() + t_index;
    this->start_routine = start_routine;
    this->arg = arg;
    this->ret_val = NULL;
    this->index = t_index;
    sigemptyset(&this->signals);
    getcontext( &this->context );
    this->stack = malloc(STACK);
    // void* stack = malloc(STACK);
    this->context.uc_stack.ss_flags = 0;
    this->context.uc_stack.ss_size = STACK;
    this->context.uc_stack.ss_sp = this->stack;
    makecontext(&this->context,wrap_start_routine,0);
    *thread = t_index ++;
    no_of_threads++;
    enQ(ready_queue,this);

    enable_timer();
    return 0;
}

void thread_exit(void *retval){
    disable_timer();
    current_thread->ret_val = retval;
    current_thread->completed = 1;
    free(current_thread->stack);
    enQ(finished_queue,current_thread);
    no_of_threads--;
    current_thread = deQ(ready_queue);

    enable_timer();
    // run next thread from ready queue
    setcontext(&current_thread->context);
}

int thread_join(thread_t thread, void **retval){
    // search for thread 
    // extract return value from tcb
    // search in ready queue first then in completed queue
    disable_timer();
    tcb* required_tcb = search_thread(ready_queue,thread);
    enable_timer();
     if(required_tcb != NULL){
    //      printf("Wait till the thread completes\n");
         while(required_tcb->completed == 0);
    //  printf("Waited for the the thread; status:- %u\n",required_tcb->completed);
    }

    disable_timer();
    required_tcb = search_thread(finished_queue,thread);
    enable_timer();
    // while(!search_thread(finished_queue,thread));
    // tcb* required_tcb = search_thread(finished_queue,thread);
    if(required_tcb == NULL){
        printf("Invalid argument to thread_join\n");
        perror("Thread doesn't exsist\n");
        return EINVAL;
    }
    if(retval){
        // printf("Stored return value\n");
        *retval = required_tcb->ret_val;
    }
    remove_thread(finished_queue,thread);
    return 0;
}

int thread_kill(thread_t thread, int sig){
    disable_timer();
    tcb* required_tcb = search_thread(ready_queue,thread);
    if(required_tcb == NULL){
        printf("Invalid argument to thread_kill\n");
        printf("Thread doesn't exsist\n");
        return 1;
    }
    sigaddset(&required_tcb->signals,sig);
    enable_timer();
}