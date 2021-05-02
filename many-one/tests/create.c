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
    init_threads();
    thread_t t;
    thread_create(&t,NULL,NULL);
    return 0;
}


