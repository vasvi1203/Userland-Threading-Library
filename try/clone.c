#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include<sys/wait.h>
#include<dirent.h>
#define STACK 8192
#define SLEEP_SEC 4
int do_something(void* arg){
        printf("Child pid : %d                  [ from getpid() ]\n", getpid());
        printf("Argument : - %d\n",*(int *)arg);
        // sleep(SLEEP_SEC);
        printf("Child is returning\n");
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
        int child_tid = clone( &do_something, (char *)stack + STACK, CLONE_VM | SIGCHLD  , (void *)arg);
        if( child_tid < 0 ){
                perror("Clone Failed");
                exit(0);
        }
        printf("Parent pid : %d                 [ from getpid() ]\n", getpid());
        printf("Child tid : %d                  [ ret-val of clone() ]\n", child_tid);
        wait(NULL);
        free(stack);
        return 0;
}