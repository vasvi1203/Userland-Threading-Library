#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<signal.h>
#include "../thread.h"

int why = 1;
int run = 1;
int run1 = 1;

void signal_handler(int sig){
    printf("Caught SIGCONT\n");
    FILE* fp = fopen("data/signal.txt","w");
    fprintf(fp,"signal SIGCONT\n");
    fclose(fp);
}
void* fun1(void* arg){
    signal(SIGCONT,signal_handler);
    // printf("[thread ]: installed singal handler\n");
    while(run);
    run1 = 0;
    thread_exit(NULL);
}

void* fun(void* arg){
    why = 0;
    run = 0;
    while(run1);
    thread_exit(NULL);
}

int main(){
    thread_t t1,t2;
    thread_create(&t1,&fun1,NULL);
    thread_create(&t2,&fun,NULL);
    // printf("Sending singal\n");
    while(why);
    thread_kill(t1,SIGCONT);
    thread_join(t1, NULL);
    thread_join(t2,NULL);
    return 0;
}