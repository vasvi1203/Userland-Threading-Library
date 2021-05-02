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

int primes[5] = {2,3,5,7,11};

void* fun(void *arg){
    int prime = *(int *)arg;
    int* ret_ptr = (int *)malloc(sizeof(int));
    *ret_ptr = prime;
    thread_exit(ret_ptr);
}

int main(){
    init_threads();
    thread_t t[5];
    void * ret;
    // printf("main thread id : %d\n",getpid());

    for(int i = 0; i < 5; i++){
        thread_create(&t[i],&fun,(primes + i));
    }
    printf("\n");
    for(int i = 0; i < 5; i++){
        thread_join(t[i],&ret);
        printf("\t %d",*(int *)ret);       
    }
   
    printf("\n");
    // printf("parent waited successfully\n");
    return 0;
}


