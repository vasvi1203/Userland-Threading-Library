#include "thread.h"
#include "queue.h"
#include<signal.h>
#include<sys/time.h>

queue* ready;
tcb* current_thread;

void scheduler(int signum, siginfo_t* info, void *context){

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
     /* when timer expires */
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 10000;
    /* Time to the next timer expiration. */
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 1;

    setitimer(ITIMER_VIRTUAL,&tick,NULL);

}

void init_threads(){
    initQ(ready);
    setup_timer();
}

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg){

}
