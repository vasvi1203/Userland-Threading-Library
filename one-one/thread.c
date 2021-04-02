#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <sys/time.h>

#include "thread.h"
#define STACK 4096
#define SLEEP_SEC 10

t_index = 0;

void thread_init() {
  tcb_table[t_index].tid = 0;
  tcb_table[t_index].child_tid = 0;
  tcb_table[t_index].parent_tid = 0;
  tcb_table[t_index].ret_val = NULL;
}

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
  void *stack = malloc(STACK);    // Stack for new process
  if(!stack) {
    perror("Malloc Failed");
    exit(EXIT_FAILURE);
  }

  // tcb init
  thread_init();
  
  int (*start)(void *) = (void *)(*start_routine);      // convert (void *)(*start_routine) to int (*start)
  tcb_table[t_index].tid = *thread = clone(start, stack + STACK,CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD | CLONE_PARENT_SETTID  | CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID, arg, NULL, NULL, &(tcb_table[t_index].child_tid));
  
  printf("clone : %lu\n",*thread);
  printf("tcb child_tid before sleep : %d\n", tcb_table[t_index].child_tid);
  printf("tcb child_tid after sleep : %d\n", tcb_table[t_index].child_tid);
  printf("tcb tid : %d\n", tcb_table[t_index].tid);

  tcb_table[t_index].parent_tid = getpid();
  printf("tcb parent_tid : %d\n", tcb_table[t_index].parent_tid);
  
  if(*thread < 0) {
    perror("Thread error\n");
    return -1;
  }
  t_index++;
  return 0;
}
