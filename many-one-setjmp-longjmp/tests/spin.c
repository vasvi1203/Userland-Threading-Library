#include "../thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

spinlock lock;
int c = 0,c1 = 0,c2 = 0,run = 1;

void* fun(void* arg){
	while(run){
		thread_spin_lock(&lock);
		c++;
		c1++;
		thread_spin_unlock(&lock);
	}
	
	thread_exit(NULL);
}

void* fun2(void* arg) {
    	while(run){
		thread_spin_lock(&lock);
		c++;
		c1++;
		thread_spin_unlock(&lock);
	}
	thread_exit(NULL);
}

int main() {
	thread_t t1,t2;
	thread_spin_init(&lock);
	thread_create(&t1,&fun,NULL);
	thread_create(&t2,&fun2,NULL);
	printf("parent is waiting\n");
	
	long i = 0;
	while(i < 1000000000) {
		i++;
	}
	
	run = 0;
	thread_join(t1, NULL);
	thread_join(t2, NULL);
	
	// printf("parent waited successfully\n");
	printf("\nChecking lock effect:-\n");
	printf("c1 = %d\n",c1);
	printf("c2 = %d\n",c2);
	printf("c1 + c2 = %d\n",c1 + c2);
	printf("c = %d\n",c);
	return 0;
}