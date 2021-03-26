#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

void* fun(void* args){
    printf("My thread\n");
    return (void*)2;
}
int main(){
    thread_t t1;
    int t = thread_create(&t1,NULL,fun,NULL);
    printf("%d\n", t);
    return 0;
}
