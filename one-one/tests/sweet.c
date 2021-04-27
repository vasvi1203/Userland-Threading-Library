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
    int *p = (int*)malloc(sizeof(int));
    *p = 20;
    sleep(2);
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(p);
}

void* fun2(void* arg){
    sleep(1);
    printf("[thread %d]: arg -----> NULL\n", gettid());
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(NULL);
}

void* fun3(void* arg){
    printf("[thread %d]: arg -----> NULL\n", gettid());
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(NULL);
}

void* fun4(void* arg){
    printf("[thread %d]: arg -----> NULL\n", gettid());
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(NULL);
}
void* fun5(void* arg){
    printf("[thread %d]: arg -----> NULL\n", gettid());
    printf("[thread %d]: exiting      \n", gettid());
    thread_exit(NULL);
}

int main(){
    thread_t t1,t2,t3,t4,t5;
    int arg = 10;
    void *ret;
    int* p = &arg;
    printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun1,(void*)p);
    thread_create(&t2,&fun2,NULL);
    thread_create(&t3,&fun3,NULL);
    thread_create(&t4,&fun4,NULL);
    thread_create(&t5,&fun5,NULL);
    printf("parent is waiting\n");
    sleep(1);
    thread_join(t1, &ret);
    thread_join(t2, NULL);
    thread_join(t3, NULL);
    thread_join(t4, NULL);
    thread_join(t5, NULL);
    int *p1 = (int *)ret;
    printf("[thread %ld]: return-value---> %d\n",t1,*p1);
    printf("parent waited successfully\n");
    return 0;
}


