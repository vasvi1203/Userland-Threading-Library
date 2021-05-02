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


void* fun1(void *arg){
    int j,ans;
    ans = 1;
    int i = *(int *)arg;
    // printf("arg :- %d\n",i);
    int* ret_ptr = (int *)malloc(sizeof(int));
    for(j = i; j > 1; j--){
        ans *= j;
    }
    *ret_ptr = ans;
    // printf("res :- %d\n",ans);
    thread_exit(ret_ptr);
}

int main(){
    thread_t t1,t2,t3;
    int *p1 = (int *)malloc(sizeof(int));
    int *p2 = (int *)malloc(sizeof(int));
    int *p3 = (int *)malloc(sizeof(int));
    *p1 = 2;
    *p2 = 3;
    *p3 = 5;
    void * ret;
    int a,b,c; 
    // printf("main thread id : %d\n",getpid());
    printf("\n\t Arguments:- 2,3 and 5\n");
    printf("\n");
    thread_create(&t1,&fun1,p1);
    thread_create(&t2,&fun1,p2);
    thread_create(&t3,&fun1,p3);
    thread_join(t1,&ret);
    a = *(int *)ret;
    printf("\t %d\n",*(int *)ret);
    thread_join(t2,&ret);
    b = *(int *)ret;
    printf("\t %d\n",*(int *)ret);
    thread_join(t3,&ret);
    c = *(int *)ret;
    printf("\t %d\n",*(int *)ret);
    printf("\n");
    free(p1);
    free(p2);
    free(p3);
    // printf("parent waited successfully\n");
    if(a == 2 && b == 6 && c == 120){
        printf("\t Passed\n");
    }
    else{
        printf("\t Failed\n");
    }
    return 0;
}


