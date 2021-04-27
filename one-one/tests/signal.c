#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>
#include "../thread.h"

void signal_handler(int sig){
    printf("Caught SIGHUP\n");
}

void* fun1(void* arg){
    signal(SIGHUP,signal_handler);
    printf("[thread %d]: arg -----> NULL\n", gettid());
    sleep(1);
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(NULL);
}


int main(){
    thread_t t1,t2,t3,t4,t5;
    printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun1,NULL);
    printf("parent is waiting\n");
    sleep(1);
    thread_kill(t1,SIGHUP);
    thread_join(t1, NULL);
    printf("parent waited successfully\n");
    return 0;
}

