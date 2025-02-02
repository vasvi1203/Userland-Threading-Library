#include "../thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//spinlock lock;
mutex m;
int c = 0,c1 = 0,c2 = 0,run = 1;

void* fun(void* arg){
	while(run){
		thread_mutex_lock(&m);
		c++;
		c1++;
		thread_mutex_unlock(&m);
	}
	thread_exit(NULL);
}

void* fun2(void* arg) {
	while(run){
		thread_mutex_lock(&m);
		c++;
		c2++;
		thread_mutex_unlock(&m);
	}
    thread_exit(NULL);
}

int main() {
	thread_t t1,t2;
	thread_mutex_init(&m);
	thread_create(&t1,&fun,NULL);
	thread_create(&t2,&fun2,NULL);
	// printf("parent is waiting\n");
	
	int i = 0;
	while(i < 100000000) {
		i++;
	}
	
	run = 0;
	thread_join(t1, NULL);
	thread_join(t2, NULL);
	
	// printf("parent waited successfully\n");
	printf("\n\t Checking lock effect:-\n");
	printf("\t c1 = %d\n", c1);
	printf("\t c2 = %d\n", c2);
	printf("\t c1 + c2 = %d\n", c1 + c2);
	printf("\t c = %d\n\n", c);

    if(c == (c1 + c2) ){
        printf("\n\t Passed\n");
    }
    else{
        printf("\n\t Failed\n");
    }
	return 0;
}
