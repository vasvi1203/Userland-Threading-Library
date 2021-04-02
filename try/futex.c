#define _GNU_SOURCE
#include <linux/futex.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include<sys/wait.h>
#include<dirent.h>
#include <errno.h>
#define STACK 8192
#define SLEEP_SEC 4

pid_t ctid;

int futex(int* uaddr, int futex_op, int val, const struct timespec* timeout,
          int* uaddr2, int val3) {
  return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

void wait_on_futex_value(int* futex_addr, int val) {
  while (1) {
    int futex_rc = futex(futex_addr, FUTEX_WAIT_PRIVATE, val, NULL, NULL, 0);
    if (futex_rc == -1) {
      if (errno != EAGAIN) {
        perror("futex");
        exit(1);
      }
    } else if (futex_rc == 0) {
      if (*futex_addr == val) {
        // This is a real wakeup.
        return;
      }
    } else {
      abort();
    }
  }
}

void wake_futex_blocking(int* futex_addr) {
//   while (1) {
    int futex_rc = futex(futex_addr, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
    if (futex_rc == -1) {
      perror("futex wake");
      exit(1);
    } else if (futex_rc > 0) {
      return;
    }
  //}
}

int do_something(void* arg){
        // syscall(SYS_futex, &ctid, FUTEX_WAIT, 0, NULL, NULL, 0);
        printf("Child pid : %d                  [ from getpid() ]\n", getpid());
        printf("Argument : - %d\n",*(int *)arg);
        // sleep(10);
        printf("Child is returning\n");
        sleep(5);
         printf("ctid in child futex: %d\n",ctid);
         sleep(5);
       // wake_futex_blocking(&ctid);
        return 0;
}


int main() {
        void *stack = malloc(STACK);    // Stack for new process
        if(!stack) {
                perror("Malloc Failed");
                exit(0);
        }
        int p = 10;
        int* arg = &p;
        int child_tid = clone( &do_something, (char *)stack + STACK, CLONE_VM | SIGCHLD | CLONE_FS| CLONE_FILES| CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID  , (void *)arg,&ctid,NULL,&ctid);
        if( child_tid < 0 ){
                perror("Clone Failed");
                exit(0);
        }
        printf("Parent waiting to start the thread\n");
        printf("ctid before futex: %d\n",ctid);
       syscall(SYS_futex, &ctid, FUTEX_WAIT, 0, NULL, NULL, 0);
        //printf("Parent waiting for child\n");
        //sleep(2);
        printf("ctid after futex: %d\n",ctid);
        // wait_on_futex_value(&ctid,child_tid);
        
        syscall(SYS_futex, &ctid, FUTEX_WAIT, child_tid, NULL, NULL, 0);
        printf("Parent waited for child successfully\n");
        free(stack);
        return 0;
}