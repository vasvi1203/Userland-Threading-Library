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
#include "thread.h"
#define STACK 4096
#define SLEEP_SEC 10

int t_index = 0;
int mutex_index = 0;
int xchg(int *addr, int newval)
{
  int result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

void print_thread(int index){
  printf("\n***************thread info: ****************\n");
  printf("\ttid\tfutex_word\tret_val\n");
  printf("\t%d\t%d\tTBD\n",tcb_table[index].tid, tcb_table[index].child_tid);
  printf("\n*******************************\n");
}

void thread_init(){
  tcb_table[t_index].tid = 0;
  tcb_table[t_index].child_tid = -1;
  tcb_table[t_index].ret_val = NULL;
}

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
  if(t_index >= MAX_THREADS){
    perror("Can't create more threads!\n");
    exit(1);
  }
  void *stack = malloc(STACK);    // Stack for new process
  if(!stack) {
    perror("Malloc Failed");
    exit(EXIT_FAILURE);
  }
  thread_init(); 
  int (*start)(void *) = (void *)(*start_routine);      // convert (void *)(*start_routine) to int (*start)
  *thread = clone(start, stack + STACK,CLONE_VM | CLONE_FS | CLONE_FILES |
                   CLONE_THREAD | CLONE_SIGHAND | CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID ,arg,NULL,NULL,&(tcb_table[t_index].child_tid ));
  if(*thread < 0) {
    perror("Thread error\n");
    return -1;
  }
  tcb_table[t_index].tid = *thread;
  t_index++;
  // print_thread(t_index -1);
  return 0;
}

int thread_join(thread_t thread, void **retval){
  int index = -1, i;

  for(i = 0; i < t_index; i++){
    if(tcb_table[i].tid == thread){
      index = i;
      break;
    }
  }

  if(index == -1 || index == t_index){
    perror("Invalid argument to thread join\n");
    exit(1);
  }    

  // print_thread(index);
  // printf("In join index: %d waiting for %d\n",index,tcb_table[index].tid);
  syscall(SYS_futex, &(tcb_table[index].child_tid), FUTEX_WAIT, -1, NULL, NULL, 0);
  syscall(SYS_futex, &(tcb_table[index].child_tid), FUTEX_WAIT, tcb_table[index].tid, NULL, NULL, 0);

  // printf("Waited successfully for index: %d for %d\n",index,tcb_table[index].tid);
 
  if(retval && tcb_table[index].ret_val) {
    *retval = tcb_table[index].ret_val;
  }
  
  for(i = index; i < t_index - 1; i++){
    tcb_table[i].child_tid = tcb_table[i + 1].child_tid;
    tcb_table[i].tid = tcb_table[i + 1].tid;
    tcb_table[i].ret_val = tcb_table[i + 1].ret_val;
  } 
  t_index--;
  printf("No. of active threads : %d\n", t_index);
}

void thread_exit(void *retval) {
  int i, thread, index = -1;
  thread = gettid();
  for(i = 0; i < t_index; i++) {
    if(tcb_table[i].tid == thread) {
      index = i;
      tcb_table[i].ret_val = retval;
      break;
    }
  }

  if(index == -1 || index == t_index){
    perror("Invalid argument to thread kill\n");
    exit(0);
  }

  // printf("%d\n", *(int *)(tcb_table[i].ret_val));
  syscall(SYS_exit, 0);
}

int thread_kill(thread_t thread, int sig) {
  int tgid = getpid();
  int i, index = -1;
  for(i = 0; i < t_index; i++) {
    if(tcb_table[i].tid == thread) {
      index = i;
      break;
    }
  }

  if(index == -1 || index == t_index){
    perror("Invalid argument to thread kill\n");
    exit(0);
  }

  tgkill(tgid, thread, sig);
}

void thread_spin_init(spinlock* spin_lock){
  spin_lock->islocked = 0;
}

void thread_mutex_init(mutex* m){
  int i;
  m->islocked = 0;
  thread_spin_init(&m->spin_lock);
  // for(i = 0; i < MAX_THREADS; i++) 
  //   m->q[i] = 0;      // RUNNABLE
}

// @credit:- xv6 code
void thread_spin_lock(spinlock* spin_lock){
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
void thread_spin_unlock(spinlock* spin_lock){
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

void thread_mutex_block(mutex *m, spinlock *sl) {
  thread_spin_unlock(sl);
  int thread = gettid();
  int i, index, sched;
  // for(i = 0; i < t_index; i++) {
  //   if(tcb_table[i].tid == thread) {
  //     index = i;
  //     break;
  //   }
  // }

  // if(index == -1 || index == t_index){
  //   perror("Invalid argument to mutex block\n");
  //   exit(0);
  // }

  // m->q[mutex_index] = tcb_table[index].tid;
  // mutex_index++;
  //syscall(SYS_futex, &(tcb_table[index].state), FUTEX_WAIT, 1, NULL, NULL, 0);
  sched = sched_yield();
  //printf("sched : %d\n", sched);
  thread_spin_lock(sl);
}

void thread_mutex_lock(mutex *m) {
  thread_spin_lock(&m->spin_lock);
  while(m->islocked)
    thread_mutex_block(m, &m->spin_lock);
  m->islocked = 1;
  thread_spin_unlock(&m->spin_lock);
}

void thread_mutex_unlock(mutex *m) {
  int i, index;
  thread_spin_lock(&m->spin_lock);
  m->islocked = 0;
  // if(mutex_index) {
  //   //printf("%d\n",mutex_index);
  //   mutex_index--;
  //   for(i = 0; i < t_index; i++) {
  //     if(tcb_table[i].tid == m->q[mutex_index]) {
  //       index = i;
  //       break;
  //     }
  //   }

  //   if(index == -1 || index == t_index){
  //     perror("Invalid argument to thread kill\n");
  //     exit(0);
  //   }
    
  //   m->q[mutex_index] = 0;
  // }
  
  thread_spin_unlock(&m->spin_lock);
}

