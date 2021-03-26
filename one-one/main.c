#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int args[4];

void* fun(void* arg){
    printf("%d\n",*(int *)arg);
}
int main(){
    thread_t tid[4];
    for(int i = 0; i < 4; i++){
        args[i] = i+1;
        thread_create(&tid[i],NULL,fun,&args[i]);
    }
    // printf("Wait for all the threads\n");
    for(int i = 0; i < 4; i++){
        // TODO: implement thread_join
        wait(NULL);
    }
    printf("Finished all the threads\n");
    
    return 0;
}
