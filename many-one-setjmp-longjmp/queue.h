#include "thread1.h"
#include<stdlib.h>

typedef struct node{
    tcb* t;
    struct node* next;
} node;

typedef struct queue{
    node *head, *tail;
    int size;
} queue;

void initQ(queue *q);
void enQ(queue *q, tcb *t);
tcb* deQ(queue *q);
void printQ(queue *q);
tcb* search_thread(queue* q, thread_t thread);
void remove_thread(queue* q, thread_t thread);