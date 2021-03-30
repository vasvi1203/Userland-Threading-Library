#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include "thread.h"
#define STACK 4096
#define SLEEP_SEC 10

// typedef struct tcb {
//     int tid;
//     pid_t* child_tid;
//     pid_t* parent_tid;
//     void* ret_val;
// } tcb;

int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
  void *stack = malloc(STACK);    // Stack for new process
  if(!stack) {
    perror("Malloc Failed");
    exit(EXIT_FAILURE);
  }

  // tcb init
  // last arg to clone :- &( tcb[1].child_tid);
  
  int (*start)(void *) = (void *)(*start_routine);      // convert (void *)(*start_routine) to int (*start)
  *thread = clone(start, stack + STACK,CLONE_VM | CLONE_FS | CLONE_FILES |
                   SIGCHLD | CLONE_PARENT_SETTID | CLONE_CHILD_SETTID |CLONE_CHILD_CLEARTID,arg,NULL,NULL,&cid);
  printf("clone : %d\n",*thread);
  printf("cid before sleep : %d\n",cid);
  sleep(1);
  printf("cid after sleep : %d\n",cid);
  if(*thread < 0) {
    perror("Thread error\n");
    return -1;
  }
  return 0;
}
