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

void* fun(void *arg){
    printf("In Thread\n");
}

int main(){
    thread_t t,t2;
    thread_create(&t,&fun,NULL);
    thread_join(t,NULL);
    thread_join(t2,NULL);
    return 0;
}


