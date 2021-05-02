#include<stdio.h>
#include<stdlib.h>

typedef struct parent{
    int a;
    int* b;
}parent;

int main(){
    parent* ptr = (parent *)malloc(sizeof(parent));
    if(ptr == NULL){
        printf("Pointer is NULL!\n");
    }
    ptr->a = 1;
    ptr->b = (int *)malloc(sizeof(int));
    *(ptr->b) = 5; 
    int* p = ptr->b;
    printf("P when parent is alive:- %d\n",*p);
    free(ptr);
    printf("P when parent is dead:- %d\n",*p);
    return 0;
}