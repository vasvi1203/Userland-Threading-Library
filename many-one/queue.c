#include "queue.h"
#include<stdio.h>
void initQ(queue* q){
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void enQ(queue* q,tcb* _t){
    // printf("EnQ:- %d\n",_t->tid);
    node* tmp = (node*)malloc(sizeof(node));
    tmp->t = _t;
    tmp->next = NULL;
    // if(q->head == NULL){
    if(q->size == 0){
        q->head = tmp;
        q->tail = tmp;
    }
    else{
        q->tail->next = tmp;
        q->tail = tmp;
    }
    // printf("EnQ:- %d\n",_t->tid);
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
    // printf("DeQ:- %d\n",tmp->tid);
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

tcb* search_thread(queue* q, thread_t t){
    if(q == NULL){
        printf("Queue NOT initialized!\n");
        return NULL;
    }
    if(q == NULL || q->head == NULL){
        // printf("Queue is empty!\n");
        return NULL;
    }

    if(q->head == NULL){
        return NULL;
    }
    node* tmp = q->head;
    // printf("Searching for: %ld\n",t);
    while(tmp && tmp->t->index != t){
        // printf("Got %d \n",tmp->t->index);
        tmp = tmp->next;
    }
    // printf("Searched for: %ld found thread:- %d\n",t,tmp->t->tid);
    tcb* res = tmp->t;
    // free(tmp);
    return res;
}

void remove_thread(queue* q, thread_t t){
    node* tmp = q->head;
    node* parent = q->head;
    while(tmp){
        if(tmp->t->index == t){
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