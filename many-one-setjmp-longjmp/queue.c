#include "thread.h"
#include<stdio.h>

void initQ(queue* q){
    q = (queue*)malloc(sizeof(queue));
    q->head = q->tail = NULL;
    q->size = 0;
}

void enQ(queue* q, tcb* t) {
    // printf("%d in enQ\n", t->tid);
    node* tmp = (node*)malloc(sizeof(node));
    tmp->t = t;
    tmp->next = NULL;

    if(q->head == NULL) {
        q->head = tmp;
        q->tail = tmp;
    }
    else {
        q->tail->next = tmp;
        q->tail = tmp;
    }
    (q->size)++;
    printQ(q);
}

tcb* deQ(queue* q) {
    tcb* tmp; 
    node* tmp_head;
    if(q->head == NULL)
        return NULL;
        
    tmp = q->head->t;
    tmp_head = q->head;
    q->head = q->head->next;
    (q->size)--;
    
    if(q->size == 0){
        q->tail = NULL;
    }
    free(tmp_head);
    return tmp;
}

void printQ(queue* q){
    node* tmp = q->head;
    while(tmp) {
        // printf("%d\n",tmp->t->tid);
        tmp = tmp->next;
    }
    // printf("end\n");
}

tcb* search_thread(queue* q, thread_t thread) {
    if(q == NULL || q->head == NULL) {
        return NULL;
    }
    tcb* tmp;
    node* tmp_head = q->head;
    while(tmp_head && tmp_head->t->tid != thread){
        tmp_head = tmp_head->next;
    }
    node *tmp1 = q->head;
    if(tmp_head) {
        q->head = tmp_head;
        tmp = q->head->t;
    }
    else
        return NULL;

    q->head = tmp1;
   
    return tmp;
}

void remove_thread(queue* q, thread_t thread) {
    if(q == NULL || q->head == NULL){
        return;
    }
    node* tmp = q->head;
    node* parent = q->head;
    while(tmp) {
        if(tmp->t->tid == thread){
            break;
        }
        parent = tmp;
        tmp = tmp->next;
    }
    if(tmp == parent){
        q->head = tmp->next;
        free(tmp);
        (q->size)--;
    }
    else if(tmp == NULL){
        printf("Invalid argument to remove_thread!\n");
        return;
    }
    else{
        parent->next = tmp->next;
        free(tmp);
        (q->size)--;
    }
}
