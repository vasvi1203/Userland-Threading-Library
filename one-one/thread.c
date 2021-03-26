#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include "thread.h"
#define STACK 4096
#define SLEEP_SEC 10

int thread_create(thread_t *thread, void *attr, void *(*start_routine)(void *), void *arg) {
  void *stack = malloc(STACK);    // Stack for new process
  if(!stack) {
    perror("Malloc Failed");
    exit(EXIT_FAILURE);
  }

  int (*start)(void *) = (void *)(*start_routine);      // convert (void *)(*start_routine) to int (*start)
  *thread = clone(start, stack + STACK, CLONE_VM, NULL);
  if(*thread < 0) {
    perror("Thread error\n");
    return -1;
  }
  return 0;
}
