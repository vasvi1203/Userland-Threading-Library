#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../thread.h"
#define NO 101

spinlock lock;
int c = 0;


void* fun1(void* arg){
        thread_spin_lock(&lock);
        c++;
        // printf("thread %d\n",c);
        thread_spin_unlock(&lock);
        thread_exit(NULL);
}



int main(){
    thread_t t[101];
    thread_spin_init(&lock);
    // printf("main thread id : %d\n",getpid());
    for(int i = 0; i < NO; i++){
        thread_create(&t[i],&fun1,NULL);
    }
     for(int i = 0; i < NO; i++){
        thread_join(t[i], NULL);
    }
    printf("\t Total threads created:- %d\n",NO);
    printf("\t Counter value:-  %d\n",c);
    if(c == NO){
        printf("\t Passed\n");
    }
    else{
        printf("\t Failed\n");
    }
    // printf("parent waited successfully\n");
    return 0;
}
