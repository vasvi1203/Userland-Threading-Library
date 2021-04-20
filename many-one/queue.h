#ifndef QUEUE_H
#define QUEUE_H 

#include "thread.h"
#include<stdlib.h>

typedef struct node{
    tcb* t;
    struct node* next;
}node;

typedef struct queue{
    node* head;
    node* tail;
    int size;
}queue;

void initQ(queue*);
void enQ(queue*,tcb*);
tcb* deQ(queue*);
void printQ(queue*);

#endif /* QUEUE_H */