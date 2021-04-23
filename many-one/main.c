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
int why = 1;
// int c = 0,c1 = 0,c2 = 0,run = 1;

void* fun(void* arg){
    printf("fun started!!!\n");
    printf("thread : received arg: %d\n", *(int *)arg);
    while(run);
    printf("thread exit : returning\n");
    // run = 0;
    int *p = (int*)malloc(sizeof(int));
    *p = 20;
    // thread_exit(p);
    printf("Run-1 = %d\n",run1);
    run1 = 0;
    printf("Run-1 = %d\n",run1);
    return p;
}

void* fun2(void* arg){
    // sleep(2);
    printf("fun2 started!!!\n");
    printf("Run = %d\n",run);
    run = 0;
    printf("Now Run = %d\n",run);
    printf("thread : received nothing\n");
    while(run1);
    printf("thread : returning\n");
    printf("why = %d\n",why);
    why = 0;
    printf("Now why = %d\n",why);
    
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
    // while(why);
    thread_create(&t1,&fun,(void*)p);
    thread_create(&t2,&fun2,NULL);
    printf("parent is waiting\n");
    // while(run);
    while(why);
    thread_join(t1,&ret);
    thread_join(t2,NULL);
    int *p1 = (int *)ret;
    printf("Return value of thread in main : %d\n",*p1);
    printf("parent waited successfully\n");
    return 0;
}
