#include "thread.h"
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

void handle_sighup(int sig){
    printf("GOT SIGHUP\n");
}

void* fun(void* arg){
	// shared resource caused race condition
	// sleep(5);
	// printf("thread : %d received arg: %d\n", gettid(), *(int *)arg);
	// printf("thread exit : %d returning\n", gettid());
	// int *p = (int*)malloc(sizeof(int));
	// *p = 20;
	int i = 0;
	//signal(SIGKILL,handle_sighup);
	// while (i < 10000)
	// {
	// 	/* code */
	// 	//printf("%d\n", i);
	// 	i++;
	// }
	
	while(run){
		thread_mutex_lock(&m);
		//thread_spin_lock(&lock);
		//printf("vwrwerew\n");
		c++;
		c1++;
		//thread_spin_unlock(&lock);
		thread_mutex_unlock(&m);
	}
	
	int *p = (int*)malloc(sizeof(int));
	*p = 20;
	thread_exit(p);
}

void* fun2(void* arg) {
	// sleep(2);
	// printf("thread : %d received nothing\n", gettid());
	// sleep(4);
	// // thread_kill(gettid(), 9);
	// printf("thread : %d returning\n", gettid());
	printf("efwdfd\n");
	while(run){
		thread_mutex_lock(&m);
		//thread_spin_lock(&lock);
		c++;
		c2++;
		//thread_spin_unlock(&lock);
		thread_mutex_unlock(&m);
	}
	return "return";
}

int main() {
	thread_t t1,t2;
	//thread_spin_init(&lock);
	thread_mutex_init(&m);
	int arg = 10;
	void *ret;
	int* p = &arg;
	printf("main thread id : %d\n",getpid());
	thread_create(&t1,&fun,NULL);
	thread_create(&t2,&fun2,NULL);
	printf("parent is waiting\n");
	
	int i = 0;
	while(i < 100000000) {
		i++;
	}
	
	run = 0;
	thread_join(t1, &ret);
	printf("%d\n", ret);
	thread_join(t2, &ret);
	printf("%s\n", ret);
	
	printf("parent waited successfully\n");
	printf("\nChecking lock effect:-\n");
	printf("c1 = %d\n",c1);
	printf("c2 = %d\n",c2);
	printf("c1 + c2 = %d\n",c1 + c2);
	printf("c = %d\n",c);
	return 0;
}
