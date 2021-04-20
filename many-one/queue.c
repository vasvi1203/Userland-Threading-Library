#include "queue.h"
#include<stdio.h>
void initQ(queue* q){
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void enQ(queue* q,tcb* t){
    node* tmp = (node*)malloc(sizeof(node));
    tmp->t = t;
    tmp->next = NULL;

    if(q->head == NULL){
        q->head = tmp;
        q->tail = tmp;
    }
    else{
        q->tail->next = tmp;
        q->tail = tmp;
    }
    printf("EnQ:- %d\n",t->tid);
    (q->size)++;
    // printQ(q);
}

tcb* deQ(queue* q){
    tcb* tmp = (tcb *)malloc(sizeof(tcb)); 
    node* tmp_head;
    if(q->size == 0){
        printf("Nothing to deQ\n");
        return NULL;
    }
    if( q->head  != NULL){
        tmp = q->head->t;
        tmp_head = q->head;
        q->head = q->head->next;
    }
    if(q->size == 1){
        q->tail = NULL;
    }
    (q->size) --;
    printf("DeQ:- %d\n",tmp->tid);
    free(tmp_head);
    return tmp;
}

void printQ(queue* q){
    node* tmp = q->head;
    while(tmp){
        printf("%d  ",tmp->t->tid);
        tmp = tmp->next;
        printf("\n");
    }
}