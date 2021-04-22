#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int run = 1;
int run1 = 1;
// int c = 0,c1 = 0,c2 = 0,run = 1;

void* fun(void* arg){
    printf("thread : received arg: %d\n", *(int *)arg);
    // while(run1);
    printf("thread exit : returning\n");
    // run = 0;
}

void* fun2(void* arg){
    // sleep(2);
    printf("thread : received nothing\n");
    printf("thread : returning\n");
    run = 0;
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
    while(run);
    printf("parent waited successfully\n");
    return 0;
}
