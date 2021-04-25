#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>

int run = 1;
int run1 = 1;
int why = 1;
// int c = 0,c1 = 0,c2 = 0,run = 1;

void handle_sighup(int sig){
    printf("GOT SIGHUP\n");
}

void* fun(void* arg){
    signal(SIGHUP,handle_sighup);
    printf("thread : received arg: %d\n", *(int *)arg);
    while(run);
    printf("thread exit : returning\n");
    // run = 0;
    int *p = (int*)malloc(sizeof(int));
    *p = 20;
    // thread_exit(p);
    run1 = 0;
    return p;
}

void* fun2(void* arg){
    why = 0;
    run = 0;
    printf("thread : received nothing\n");
    while(run1);
    printf("thread : returning\n");
}

int main(){
    thread_t t1,t2;
    //init_spin_lock(&lock);
    // thread_mutex_init(&m);
    init_threads();
    int arg = 10;
    void *ret;
    int* p = &arg;
    printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun,(void*)p);
    thread_create(&t2,&fun2,NULL);
    printf("parent is waiting\n");
    // // while(run);
    while(why);
    thread_kill(t1,SIGHUP);
    thread_join(t1,&ret);
    thread_join(t2,NULL);
    int *p1 = (int *)ret;
    printf("Return value of thread in main : %d\n",*p1);
    printf("parent waited successfully\n");
    return 0;
}
