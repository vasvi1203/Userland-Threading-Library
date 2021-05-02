// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include<signal.h>
// #include "thread.h"
// #include "lock.h"


// int why = 1;
// int c = 0,c1 = 0,c2 = 0,run = 1;

// spinlock lock;

// void handle_sighup(int sig){
//     printf("GOT SIGHUP\n");
// }

// void* fun1(void* arg){
//     // signal(SIGHUP,handle_sighup);
//     printf("thread : received arg: %d\n", *(int *)arg);
//     while(run){
//         thread_spin_lock(&lock);
//         c++;
//         c1++;
//         thread_spin_unlock(&lock);
//     }
//     printf("thread exit : returning\n");
//     // run = 0;
//     int *p = (int*)malloc(sizeof(int));
//     *p = 20;
//     // thread_exit(p);
//     return p;
// }

// int tno = 0;

// void* fun(void* arg){
//     printf("%d\n",tno++);
//     thread_exit(NULL);
// }

// void* fun2(void* arg){
//     printf("Now why = %d\n",why);
//     printf("thread : received nothing\n");
//      while(run){
//         thread_spin_lock(&lock);
//         c++;
//         c2++;
//         thread_spin_unlock(&lock);
//     }
//     printf("thread : returning\n");
    
// }

// void* fun3(void* arg){
//     why = 0;
//     printf("DUMMY thread : returning\n");
    
// }

// int main(){
//     thread_t t[100];
//     // thread_spin_init(&lock);
//     init_threads();
//     int arg = 10;
//     void *ret;
//     int* p = &arg;
//     printf("main thread id : %d\n",getpid());
//     // thread_create(&t1,&fun,(void*)p);
//     // thread_create(&t2,&fun2,NULL);
//     // thread_create(&t3,&fun3,NULL);
//     for(int i = 0; i < 101; i++){
//         thread_create(&t[i],&fun,NULL);
//     }
//     // printf("parent is waiting\n");
//     // while(why);
//     // printf("In main:- why = %d\n",why);
//     // run = 0;
//     // printf("In main:- run = %d\n",run);
//     // thread_kill(t1,SIGHUP);
//     // thread_join(t1,&ret);
//     // thread_join(t2,NULL);
//     // thread_join(t3,NULL);
//     for(int i = 0; i < 101; i++){
//         thread_join(t[i],NULL);
//     }
//     // int *p1 = (int *)ret;
//     // printf("Return value of thread in main : %d\n",*p1);
//     printf("parent waited successfully\n");
//     // printf("checking lock effect:-\n");
//     // printf("c1 :%d\n",c1);
//     // printf("c2 :%d\n",c2);
//     // printf("c1+c2 :%d\n",c1+c2);
//     // printf("c :%d\n",c);
//     return 0;
// }


#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include "thread.h"
#define MAX 100000

int** a;
int** b;
int** c;
int m,n,p,q;
int step = 0;

// utility function
void print_matrix(){
    // print the size first
    printf("%d %d\n",m,q);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < q; j++){
            printf("%d ",c[i][j]);
        }
        printf("\n");
    }
}

// thread function
void* matmul(){
    // Each thread to do 1/3rd job
    // store the step locally in each thread to avoid interference with other threads
    // each "core" will execute seperately
    // 3 cores for concurrent multiplication
    // printf("Thread: - %d",getpid());
    int core = step++;
    int start =  core*m/3;
    int end = (core+1)*m/3;
    int i,j,k;
    for(i = start; i < end; i++){
        for(j = 0; j < q; j++){
            c[i][j] = 0;
            for(k = 0; k < n; k++){
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }
    thread_exit(NULL);
}
int main(){
    // three threads
    init_threads();
    thread_t t[3];
    int i,j,k;

    // Matrix A
    // printf("enter numbers a:-\n");
    scanf("%d%d",&m,&n);
    a = (int **)malloc(m*sizeof(int *));
    for(i = 0; i < m; i++){
        a[i] = (int *)malloc(n*sizeof(int));
    }
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%d",&a[i][j]);
        }
    }

    // Matrix B
    // printf("enter numbers b:-\n");
    scanf("%d%d",&p,&q);
    b = (int **)malloc(p*sizeof(int *));
    for(i = 0; i < p; i++){
        b[i] = (int *)malloc(q*sizeof(int));
    }
    for(i = 0; i < p; i++){
        for(j = 0; j < q; j++){
            scanf("%d",&b[i][j]);
        }
    }

    // Invalid dimensions for matrix multiplication
    if(n != p){
        printf("-1\n");
        return 0;
    }
    
    // Matrix C
    c = (int **)malloc(m*sizeof(int *));
    for(i = 0 ; i < m; i++){
        c[i] = (int *)malloc(sizeof(int)*q);
    }

    // threads
    for(i = 0; i < 3; i++){
        thread_create(&t[i],matmul,NULL);
    }
    for(int i = 0; i < 3; i++){
        thread_join(t[i],NULL);
    }
    printf("\n");
    print_matrix();

    // free
    for(i = 0; i < m; i++){
        free(a[i]);
        free(c[i]);
    }
    for(i = 0; i < p; i++){
        free(b[i]);
    }
    free(a);
    free(b);
    free(c);
    
    return 0;
}
