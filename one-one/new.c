#include<stdio.h>
#include<pthread.h>
#define N 110

int c = 0;

void* fun(void* arg){
    // printf("%d\n",c++);
    c++;
    pthread_exit(NULL);
}

int main(){
    pthread_t t[105];
    for(int i = 1; i < N; i++){
        pthread_create(&t[i],NULL,&fun,NULL);
    }
    for(int i = 0; i < N; i++){
        pthread_join(t[i],NULL);
    }
    printf("Done %d\n",c);
    return 0;
}