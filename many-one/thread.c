#include "thread.h"
#include "queue.h"
#include<signal.h>
#include<string.h>
#include<sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>

queue* ready_queue;
queue* finished_queue;
tcb* current_thread;
int t_index = 1;

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

void scheduler(int signum, siginfo_t* info, void *context){
    // curretnly running thread --> enQ
    // deQ thread from ready --> thread created by user
    // make it running --> run that thread
    // makecontext(running)
    printf("TIMER INTR\n");
    ucontext_t* prev_thread = &current_thread->context;
    ucontext_t* returned_from = (ucontext_t *)context; 

    prev_thread->uc_flags = returned_from->uc_flags;
    prev_thread->uc_mcontext = returned_from->uc_mcontext;
    prev_thread->uc_sigmask = returned_from->uc_sigmask;

    enQ(ready_queue,current_thread);
    
    current_thread = deQ(ready_queue);
   
    setcontext(&current_thread->context);
}

void setup_timer(){
    // array representing signals
    sigset_t signal_set;
    // set all entries in the array
    sigfillset(&signal_set);

    struct sigaction ticker;
    ticker.sa_sigaction = scheduler;  // signal handler
    ticker.sa_mask = signal_set;      
    ticker.sa_flags = 0;

    // SIGVTALRM suits more than SIGALRM
    // handle SIGVTALRM with ticker configurations
    sigaction(SIGVTALRM,&ticker,NULL);

    struct itimerval tick;
    // //  /* when timer expires */
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 5000;
    // // /* Time to the next timer expiration. */
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 1;

    setitimer(ITIMER_VIRTUAL,&tick,NULL);
    printf("TIMER INIT\n");
}

void make_main_thread_context(){
    tcb* main_thread = (tcb *)malloc(sizeof(tcb));
    main_thread->tid = getpid();
    getcontext(&main_thread->context);
    // strcpy(main_thread->state,"RUNNING");
    current_thread = main_thread;
}

void init_threads(){
    ready_queue = (queue *)malloc(sizeof(queue));
    finished_queue = (queue *)malloc(sizeof(queue));
    initQ(ready_queue);
    initQ(finished_queue);
    setup_timer();
    // sleep(2);
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
    tcb* this = (tcb *)malloc(sizeof(tcb));
    this->tid = getpid() + t_index;
    // strcpy( this->state, "READY");
    this->start_routine = start_routine;
    this->arg = arg;
    this->ret_val = NULL;
    getcontext( &this->context );
    void* stack = malloc(STACK);
    this->context.uc_stack.ss_flags = 0;
    this->context.uc_stack.ss_size = STACK;
    this->context.uc_stack.ss_sp = stack;
    makecontext(&this->context,wrap_start_routine,1);
    *thread = t_index ++;
    enQ(ready_queue,this);
    enable_timer();
}

void thread_exit(void *retval){
    disable_timer();
    current_thread->ret_val = retval;
    // strcpy(current_thread->state,"COMPLETED");
    enQ(finished_queue,current_thread);
    current_thread = deQ(ready_queue);
    enable_timer();
    // run next thread from ready queue
    setcontext(&current_thread->context);
}