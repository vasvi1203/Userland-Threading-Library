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

spinlock lock;


int c = 0;
int c1 = 0;
int c2 = 0;
int c3 = 0;
int c4 = 0;
int run = 1;

void* fun1(void* arg){
     while(run){
        thread_spin_lock(&lock);
        c++;
        c1++;
        thread_spin_unlock(&lock);
    }
    thread_exit(NULL);
}

void* fun2(void* arg){
    while(run){
        thread_spin_lock(&lock);
        c++;
        c2++;
        thread_spin_unlock(&lock);
    }
    thread_exit(NULL);
}

void* fun3(void* arg){
    while(run){
        thread_spin_lock(&lock);
        c++;
        c3++;
        thread_spin_unlock(&lock);
    }
    thread_exit(NULL);
}

void* fun4(void* arg){
    while(run){
        thread_spin_lock(&lock);
        c++;
        c4++;
        thread_spin_unlock(&lock);
    }
    thread_exit(NULL);
}


int main(){
    thread_t t1,t2,t3,t4;
    thread_spin_init(&lock);
    // printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun1,NULL);
    thread_create(&t2,&fun2,NULL);
    thread_create(&t3,&fun3,NULL);
    thread_create(&t4,&fun4,NULL);
    // printf("parent is waiting\n");
    sleep(1);
    run = 0;
    thread_join(t1, NULL);
    thread_join(t2, NULL);
    thread_join(t3, NULL);
    thread_join(t4, NULL);
    // printf("parent waited successfully\n");
    printf("\n\t Checking lock effect:-\n");
    printf("\t c1 = %d\n",c1);
    printf("\t c2 = %d\n",c2);
    printf("\t c3 = %d\n",c3);
    printf("\t c4 = %d\n",c4);
    printf("\t c1 + c2 + c3 + c4 = %d\n",c1 + c2 + c3 + c4);
    printf("\t c = %d\n",c);
    if(c == (c1 + c2 + c3 + c4)){
        printf("\n\t Passed\n");
    }
    else{
        printf("\n\t Failed\n");
    }
    return 0;
}
