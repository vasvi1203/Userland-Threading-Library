#define _GNU_SOURCE
#include <linux/futex.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>  
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <setjmp.h>
//#include "thread1.h"
#include "queue.h"
#define STACK 32767
#define SLEEP_SEC 10

queue ready;
queue finished_queue;
tcb* current_thread;
jmp_buf SCHEDULER;

int init = 0;

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
	// currently running thread --> enQ
	// deQ thread from ready --> thread created by user
	// make it running --> run that thread
	// makecontext(running)
	printf("TIMER INTR\n");
	
	if(setjmp(current_thread->state) == 0) {
		enQ(&ready, current_thread);
		current_thread = deQ(&ready);
		printf("sched %ld\n", current_thread->state->__jmpbuf[6]);
		longjmp(current_thread->state, 1);
	}
	else
		return;
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
    // tick after every 10000 usec
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 1;
    // tick for 1 usec for the first time
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 1;

    setitimer(ITIMER_VIRTUAL,&tick,NULL);
    printf("TIMER INIT\n");
}

static long int ptr_mangle(long int var) {
	long int ret;
	asm(" mov %1, %%rax;\n"
		" xor %%fs:0x30, %%rax;"
		" rol $0x11, %%rax;"
		" mov %%rax, %0;"
	: "=r"(ret)
	: "r"(var)
	: "%rax"
	);
	return ret;
}

void make_main_thread_context(void){
	printf("Main\n");
  	tcb* main_thread = (tcb*)malloc(sizeof(tcb));
	void *stack = malloc(STACK);
	if(main_thread == NULL) {
		perror("Malloc Failed");
		exit(EXIT_FAILURE);
	}

	main_thread->tid = getpid();
	printf("main %ld\n", main_thread->state->__jmpbuf[6]);
	main_thread->stack = stack;
	main_thread->start_routine = NULL;
	main_thread->arg = NULL;
	main_thread->ret_val = NULL;
	init = 1;
	current_thread = main_thread;
}

void thread_init(void) {
	if(init == 0) {
		initQ(&ready);
		initQ(&finished_queue);
		setup_timer();
		make_main_thread_context();
		return;
	}    
	tcb *t = (tcb*)malloc(sizeof(tcb));
	if(t == NULL) {
		perror("Malloc Failed");
		exit(EXIT_FAILURE);
	}
	
	t->tid = ready.size + 1;
	t->ret_val = NULL;

	enQ(&ready, t);
}


void thread_exit(void *retval){
    disable_timer();
    current_thread->ret_val = retval;
    //current_thread->completed = 1;
	printf("before enQ%d\n", current_thread->tid);
    enQ(&finished_queue,current_thread);
    //no_of_threads--;
    printQ(&ready);
    current_thread = deQ(&ready);
	printf("after enQ%d\n", current_thread->tid);

    enable_timer();
    // run next thread from ready queue
	//longjmp(current_thread->state, 1);    
}

void wrap_start_routine(){
	printf("trvey\n");
    disable_timer();
    tcb* run = current_thread;
    enable_timer();
    void* result = run->start_routine(run->arg);
    thread_exit(result);
}

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
	disable_timer();
	if(init != 0) {
		if (ready.size >= MAX_THREADS){
			perror("Can't create more threads!\n");
			exit(1);
		}
	}

	void *stack = malloc(STACK);    // Stack for new thread
	
	if(!stack) {
		perror("Malloc Failed");
		exit(EXIT_FAILURE);
	}

	if(init == 0) 
		thread_init();

	thread_init();

	*thread = ready.tail->t->tid;
	ready.tail->t->start_routine = start_routine;  
	ready.tail->t->arg = arg;
	ready.tail->t->stack = stack;

	if(setjmp(ready.tail->t->state) == 0) 
		printf("Saved\n");
	else
		return 0;

	long int start = (long int)(wrap_start_routine); 
	ready.tail->t->state->__jmpbuf[7] = ptr_mangle(start);
	printf("start %ld\n", ready.tail->t->state->__jmpbuf[7]);

	long int stack1 = (long int)stack; 
	ready.tail->t->state->__jmpbuf[6] = ptr_mangle(stack1);
	printf("stack %ld\n", ready.tail->t->state->__jmpbuf[6]);

	// start_routine(arg);
	
	enable_timer();
	
	return 0;
}

