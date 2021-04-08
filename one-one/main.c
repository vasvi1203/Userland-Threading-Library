#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void* fun(void* arg){
    // shared resource caused race condition
    sleep(5);
    printf("thread : %d received arg: %d\n", getpid(), *(int *)arg);
    printf("thread exit : %d returning\n", gettid());
    int *p = (int*)malloc(sizeof(int));
    *p = 20;
    thread_exit(p);
}

void* fun2(void* arg){
    sleep(2);
    printf("thread : %d received nothing\n", gettid());
    sleep(4);
    thread_kill(gettid(), 9);
    printf("thread : %d returning\n", gettid());
}

int main(){
    thread_t t1,t2;
    // setbuf(stdout,NULL);
    int arg = 10;
    int* p = &arg;
    printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun,(void*)p);
    thread_create(&t2,&fun2,NULL);
    printf("parent is waiting\n");
    thread_join(t1);
    thread_join(t2);
    // sleep(7);
    printf("parent waited successfully\n");
    return 0;
}
