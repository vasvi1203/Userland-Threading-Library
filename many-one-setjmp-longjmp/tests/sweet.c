#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../thread.h"


void* fun1(void *arg){
    int *p = (int*)malloc(sizeof(int));
    *p = ( *(int *)arg ) * ( *(int *)arg  ) ;
    thread_exit(p);
}

int main(){
    thread_t t1,t2,t3;
    void * ret;
    int arg = 18;
    printf("\t Arg:\t %d",arg);
    int* p = &arg;
    int a,b,c; 
    thread_create(&t1,&fun1,(void *)p);
    thread_join(t1,&ret);
    int *p1 = (int *)ret;
    printf("\t Ret:\t %d\n\n",*p1);
    if(*p1 == arg*arg){
        printf("\t Passed\n");
    }
    else{
        printf("\t Failed\n");
    }
    return 0;
}
