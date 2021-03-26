#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void* fun(void* args){
    printf("My thread\n");
}
int main(){
    thread_t t1,t2,t3,t4;
    thread_create(&t1,NULL,fun,NULL);
    thread_create(&t2,NULL,fun,NULL);
    thread_create(&t3,NULL,fun,NULL);
    thread_create(&t4,NULL,fun,NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    printf("Parent\n");
    return 0;
}
