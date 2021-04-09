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

void print_thread(int index){
  printf("\n***************thread info: ****************\n");
  printf("\ttid\tfutex_word\tret_val\n");
  printf("\t%d\t%d\tTBD\n",tcb_table[index].tid, tcb_table[index].child_tid);
  printf("\n*******************************\n");
}

void thread_init(){
  tcb_table[t_index].tid = 0;
  tcb_table[t_index].child_tid = -1;
  // tcb_table[t_index].parent_tid = -1;
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
  // printf("before%d\n", t_index);
  // t_index--;
  // for(i = index; i < t_index; i++) 
  //   tcb_table[i] = tcb_table[i + 1];

  // printf("after%d\n", t_index);
}

void thread_exit(void *retval) {
  int i, thread, index = -1;
  thread = gettid();
  for(i = 0; i < t_index; i++) {
    if(tcb_table[i].tid == thread) {
      index = i;
      tcb_table[i].ret_val = retval;
      printf("enter\n");
      break;
    }
  }

  if(index == -1 || index == t_index){
    perror("Invalid argument to thread kill\n");
    exit(0);
  }

  printf("%d\n", *(int *)(tcb_table[i].ret_val));
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