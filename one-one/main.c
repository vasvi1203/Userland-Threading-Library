#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int args[4];

void* fun(void* arg){
    // shared resource caused race condition
    printf("tid : %d\n",getpid());
    printf("parent id : %d\n",getppid());
    printf("thread recevied arg: %d\n",*(int *)arg);
    
}
int main(){
    thread_t t1;
    int arg = 10;
    int* p = &arg;
     printf("main thread id : %d\n",getpid());
    thread_create(&t1,&fun,p);
    wait(NULL);
    return 0;
}
