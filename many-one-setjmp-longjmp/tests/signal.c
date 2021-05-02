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

void signal_handler(int sig){
    // printf("Caught SIGHUP\n");
    FILE* fp = fopen("data/signal.txt","w");
    fprintf(fp,"signal\n");
    fclose(fp);
}
void* fun1(void* arg){
    signal(SIGHUP,signal_handler);
    // printf("[thread %d]: installed singal handler\n",gettid());
    sleep(2);
    thread_exit(NULL);
}
int main(){
    thread_t t1;
    thread_create(&t1,&fun1,NULL);
    sleep(1);
    // printf("Sending singal\n");
    thread_kill(t1,SIGHUP);
    thread_join(t1, NULL);
    return 0;
}