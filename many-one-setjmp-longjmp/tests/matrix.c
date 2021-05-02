#define _GNU_SOURCE
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include "../thread.h"
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
   // printf("\n");
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