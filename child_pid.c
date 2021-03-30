#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t ctid = 0;

int child_fn(void * arg)
{
	sleep(1);
	printf("child, ctid: %d\n", ctid);
	sleep(2);
	return 0;
}

int main(void)
{
	void * stack = NULL;
	stack = malloc(0x1000);
	clone(child_fn, stack + 0x1000 - 0x8, CLONE_VM | CLONE_CHILD_SETTID,
			NULL, NULL, NULL, &ctid);
	printf("parent, ctid: %d\n", ctid);
	sleep(2);
	printf("parent, ctid: %d\n", ctid);
	return 0;
}