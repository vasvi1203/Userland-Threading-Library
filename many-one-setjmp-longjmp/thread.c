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
#include <errno.h>
#include <sys/mman.h>
#include "thread.h"
#define STACK 32768

queue thread_queue;
tcb* current_thread;

int init = 0;

void disable_timer(void) {
    sigset_t sigvtalrm;
    sigemptyset(&sigvtalrm);
    sigaddset(&sigvtalrm, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &sigvtalrm, NULL);
}

void enable_timer(void) {
    sigset_t sigvtalrm;
    sigemptyset(&sigvtalrm);
    sigaddset(&sigvtalrm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &sigvtalrm, NULL);
}

void deq_ready_thread() {
    node *head = thread_queue.head;
	while(head) {
		if(head->t->status != EXITED && head->t->status != WAITING) {
			current_thread = head->t;
			remove_thread(&thread_queue, head->t->tid);
    		break;
		}
		head = head->next;
	}
}

void scheduler(int signum){
	// currently running thread --> enQ
	// deQ ready thread --> thread created by user
	// change status to running --> run that thread
	// longjmp(running)
	disable_timer();
	int i;
	
	if(setjmp(current_thread->state) == 0) {
	
		if(current_thread->status != EXITED && current_thread->status != WAITING)
			current_thread->status = READY;

		enQ(&thread_queue, current_thread);
		
		deq_ready_thread();
		if(current_thread == NULL)
			exit(0);

		current_thread->status = RUNNING;
		
		enable_timer();
		for(i = 1; i < 32; i++) {
			if(sigismember(&current_thread->signals, i)) {
				raise(i);
				sigdelset(&current_thread->signals,i);
			}
		}

		longjmp(current_thread->state, 1);
	}
	else
		enable_timer();
		return;
}

void thread_exit(void *retval){
	disable_timer();
    current_thread->ret_val = retval;
	current_thread->status = EXITED;
	enable_timer(); 
}

void wrap_start_routine() {
    void* result = current_thread->start_routine(current_thread->arg);
	if(result != NULL) {
		thread_exit(result);
	}
	scheduler(1);
}

void setup_timer() {
    // array representing signals
    sigset_t signal_set;
    // set all entries in the array
    sigfillset(&signal_set);

    struct sigaction ticker;
    ticker.sa_handler = scheduler;  // signal handler
    ticker.sa_mask = signal_set;      
    ticker.sa_flags = 0;

    // SIGVTALRM suits more than SIGALRM
    // handle SIGVTALRM with ticker configurations
    sigaction(SIGVTALRM,&ticker,NULL);

	// @credit:- https://stackoverflow.com/questions/48064940/understanding-struct-itimerval-field-tv-usec
    struct itimerval tick;
    // tick for 1 usec for the first time
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 1;
    // tick after every 10000 usec
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 10000;

    setitimer(ITIMER_VIRTUAL,&tick,NULL);
}

void make_main_thread_context(void){
  	tcb* main_thread = (tcb*)malloc(sizeof(tcb));
	if(main_thread == NULL) {
		perror("Malloc Failed");
		exit(EXIT_FAILURE);
	}
	main_thread->tid = getpid();
	main_thread->start_routine = NULL;
	main_thread->arg = NULL;
	main_thread->ret_val = NULL;
	main_thread->status = RUNNING;
	main_thread->stack = NULL;
    sigemptyset(&main_thread->signals);
	init = 1;
	current_thread = main_thread;
}

void thread_init(void) {
	if(init == 0) {
		initQ(&thread_queue);
		make_main_thread_context();
		setup_timer();
		return;
	}    
	tcb *t = (tcb*)malloc(sizeof(tcb));
	if(t == NULL) {
		perror("Malloc Failed");
		exit(EXIT_FAILURE);
	}
	
	t->tid = thread_queue.size + 1;
	t->ret_val = NULL;
	t->status = READY;
	t->arg = NULL;
	t->start_routine = NULL;
	t->stack = NULL;

	enQ(&thread_queue, t);
	return;
}

// @credit:- https://sites.cs.ucsb.edu/~chris/teaching/cs170/projects/proj2.html
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

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
	disable_timer();

	// Initially when main is run, then thread_queue not initialized
	if(init != 0) {
		if (thread_queue.size >= MAX_THREADS){
			perror("Can't create more threads!\n");
			exit(1);
		}
	}

	if(start_routine == NULL){
		perror("Invalid start routine!\n");
		return EINVAL;
	}
	
	// Stack for new thread
	void *stack = mmap(NULL, STACK, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	
	if (stack == MAP_FAILED)
		exit(2);

	// for main thread
	if(init == 0) 
		thread_init();

	// for other user threads
	thread_init();

	*thread = thread_queue.tail->t->tid;
	thread_queue.tail->t->start_routine = start_routine;  
	thread_queue.tail->t->arg = arg;
	thread_queue.tail->t->stack = stack;

	if(setjmp(thread_queue.tail->t->state) == 1) {
		return 0;
	}
	
	long int start = (long int)(wrap_start_routine); 
	thread_queue.tail->t->state->__jmpbuf[7] = ptr_mangle(start);

	long int stack1 = (long int)(stack + STACK); 
	thread_queue.tail->t->state->__jmpbuf[6] = ptr_mangle(stack1);
	
	sigemptyset(&thread_queue.tail->t->signals);
	enable_timer();
	
	return 0;
}

int thread_kill(thread_t thread, int sig) {
    disable_timer();
	if(sig < 0 || sig > 32) {
		printf("Invalid signal sent!\n");
		return EINVAL;
	}

    tcb* required_tcb = search_thread(&thread_queue, thread);
    if(required_tcb == NULL) {
        printf("Invalid argument to thread_kill\n");
        printf("Thread doesn't exist\n");
        return EINVAL;
    }
    sigaddset(&required_tcb->signals, sig);
    enable_timer();
}

int thread_join(thread_t thread, void **retval) {
    // search for thread 
    // extract return value from tcb
    disable_timer();
    tcb* required_tcb = search_thread(&thread_queue,thread);
    enable_timer();
	if(required_tcb == NULL) {
        printf("Invalid argument to thread_join\n");
        printf("Thread doesn't exist\n");
        return EINVAL;
    }
    if(required_tcb != NULL) {
        while(required_tcb->status != EXITED);
    }
    
    if(retval) {
        *retval = required_tcb->ret_val;
    }
    remove_thread(&thread_queue,thread);
    return 0;
}

// @credit:- xv6 code
int xchg(int *addr, int newval) {
  int result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

// @credit:- xv6 code
void thread_spin_init(spinlock* spin_lock) {
  spin_lock->islocked = 0;
}

// @credit:- OS lecture
void thread_mutex_init(mutex* m) {
  m->islocked = 0;
  thread_spin_init(&m->spin_lock);
  initQ(&m->wait_queue);
}

// @credit:- xv6 code
void thread_spin_lock(spinlock* spin_lock) {
  // The xchg is atomic.
  while(xchg(&(spin_lock->islocked), 1) != 0)
    ;
  
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();

}

// @credit:- xv6 code
void thread_spin_unlock(spinlock* spin_lock) {
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
   __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (spin_lock->islocked) : );
}

// @credit:- OS lecture
void thread_mutex_block(mutex *m, spinlock *sl) {
  thread_spin_unlock(sl);
  current_thread->status = WAITING;
  enQ(&m->wait_queue, current_thread);
  scheduler(1);
  thread_spin_lock(sl);
}

// @credit:- OS lecture
void thread_mutex_lock(mutex *m) {
  thread_spin_lock(&m->spin_lock);
  while(m->islocked)
    thread_mutex_block(m, &m->spin_lock);
  m->islocked = 1;
  thread_spin_unlock(&m->spin_lock);
}

// @credit:- OS lecture
void thread_mutex_unlock(mutex *m) {
  thread_spin_lock(&m->spin_lock);
  m->islocked = 0;
  tcb *some_thread = deQ(&m->wait_queue);
  if(some_thread) {
	some_thread->status = READY;
  }
  thread_spin_unlock(&m->spin_lock);
} 
