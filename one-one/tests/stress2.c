#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../thread.h"

void* fun1(void* arg){
    printf("[thread %d]: arg -----> %d\n", gettid(), *(int *)arg);
    printf("[thread %d]: exiting      \n", gettid());
    FILE* fp;
    fp = fopen("stress2.txt","w");
    fwrite("hello",1,5,fp);
    fclose(fp);
}


int main(){
    thread_t t1,t2,t3,t4,t5;
    int arg = 10;
    void *ret;
    int* p = &arg;
    printf("main thread id : %d\n",getpid());
    // FILE* fp;
    // fp = fopen("data/stress2.txt","w");
    // fclose(fp);
    thread_create(&t1,&fun1,(void*)p);
    printf("parent is waiting\n");
    sleep(1);
    thread_join(t1, &ret);
    printf("parent waited successfully\n");
    return 0;
}


