#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include<sys/wait.h>
#include<dirent.h>
#define STACK 8192
#define SLEEP_SEC 3
int do_something(){
        printf("Child pid : %d\n", getpid());
        sleep(SLEEP_SEC);
        printf("Child is returning\n");
        return 0;
}
int main() {
        void *stack = malloc(STACK);    // Stack for new process
        if(!stack) {
                perror("Malloc Failed");
                exit(0);
        }
        int child_tid = clone( &do_something, (char *)stack + STACK, CLONE_VM | CLONE_PARENT , 0);
        if( child_tid < 0 ){
                perror("Clone Failed");
                exit(0);
        }
        printf("Parent pid : %d\n", getpid());
        printf("Child tid : %d\n", child_tid);
        // sleep(10); // Add sleep so we can see both processes output
        DIR *proc_dir;
    {
        char dirname[100];
        snprintf(dirname, sizeof dirname, "/proc/%d/task", getpid());
        proc_dir = opendir(dirname);
    }

    if (proc_dir)
    {
        /* /proc available, iterate through tasks... */
        struct dirent *entry;
        while ((entry = readdir(proc_dir)) != NULL)
        {
            if(entry->d_name[0] == '.')
                continue;

            int tid = atoi(entry->d_name);

            /* ... (do stuff with tid) ... */
            printf("thread id : %d\n",tid);
        }

        closedir(proc_dir);
    }
    // else
    // {
    //     /* /proc not available, act accordingly */
    //     printf("Nasty\n");
    // }

    sleep(SLEEP_SEC);
        free(stack);
        return 0;
}