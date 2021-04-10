#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

spinlock lock;
mutex m;
int c = 0,c1 = 0,c2 = 0,run = 1;

void* fun(void* arg){
    // shared resource caused race condition
    // sleep(5);
    // printf("thread : %d received arg: %d\n", gettid(), *(int *)arg);
    // printf("thread exit : %d returning\n", gettid());
    // int *p = (int*)malloc(sizeof(int));
    // *p = 20;
    while(run){
        acquire(&m);
        c++;
        c1++;
        release(&m);
    }
    
    int *p = (int*)malloc(sizeof(int));
    *p = 20;
    thread_exit(p);
}

void* fun2(void* arg){
    // sleep(2);
    // printf("thread : %d received nothing\n", gettid());
    // sleep(4);
    // // thread_kill(gettid(), 9);
    // printf("thread : %d returning\n", gettid());
    while(run){
        acquire(&m);
        c++;
        c2++;
        release(&m);
    }
}

int main(){
    thread_t t1,t2;
    //init_spin_lock(&lock);
    init_mutex(&m);
    int arg = 10;
    void *ret;
    int* p = &arg;
    printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun,(void*)p);
    thread_create(&t2,&fun2,NULL);
    printf("parent is waiting\n");
    sleep(1);
    run = 0;
    thread_join(t1, &ret);
    thread_join(t2, NULL);
    int *p1 = (int *)ret;
    printf("In main: %d\n",*p1);
    
    printf("parent waited successfully\n");
    printf("\nChecking lock effect:-\n");
    printf("c1 = %d\n",c1);
    printf("c2 = %d\n",c2);
    printf("c1 + c2 = %d\n",c1 + c2);
    printf("c = %d\n",c);
    return 0;
}
