#include<stdlib.h>
#include<stdio.h>
#include "queue.h"

int main(){
    queue Q;
    initQ(&Q);
    tcb t1,t2,t3,t4,*ret;
    t1.tid = 1;
    t2.tid = 2;
    t3.tid = 3;
    t4.tid = 4;

    enQ(&Q,&t1);
    printQ(&Q);
    ret = deQ(&Q);
    printf("DEQ: %d\n",ret->tid);
    enQ(&Q,&t2);
    enQ(&Q,&t3);
    enQ(&Q,&t4); 
    printQ(&Q);
    ret = deQ(&Q);
    printf("DEQ: %d\n",ret->tid);
    return 0;
}