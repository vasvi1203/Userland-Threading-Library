# Userland Threading Library

A lightweight threading library written in C that demonstrates how threads are created, scheduled, synchronized, and managed on Linux.

The project implements both **one-to-one** and **many-to-one** threading models using low-level operating-system concepts such as:

* Linux `clone()`
* Futexes
* Signals and timers
* Context switching
* `ucontext`
* `setjmp()` and `longjmp()`
* Spinlocks and mutexes

> This project is intended for systems-programming education and experimentation rather than production use.

## Features

* One-to-one and many-to-one threading models
* User-space preemptive scheduling
* Thread creation, termination, and joining
* Independent stacks for individual threads
* Thread control blocks and scheduling queues
* Spinlock and mutex synchronization
* Futex-based blocking
* Signal delivery
* Functional, synchronization, and stress tests

## Threading Models

### One-to-One

In the one-to-one model, each library thread maps to a separate Linux kernel thread.

The implementation uses:

* Linux `clone()` for thread creation
* Shared memory and file descriptors
* Futexes for thread joining and mutex blocking
* Atomic operations for spinlocks
* `tgkill()` for signal delivery

Because every user thread maps to a kernel thread, multiple threads can run concurrently across different CPU cores.

### Many-to-One Using `ucontext`

In this model, multiple user-level threads run inside a single kernel thread.

The implementation uses:

* `getcontext()`
* `makecontext()`
* `swapcontext()`
* `setcontext()`
* A user-space ready queue
* Timer-based preemptive scheduling
* Separate stacks and thread control blocks

A virtual timer periodically interrupts the running thread and transfers control to the scheduler, which selects the next runnable thread.

### Many-to-One Using `setjmp()` and `longjmp()`

This implementation performs user-space context switching using:

* `setjmp()`
* `longjmp()`
* Manually allocated stacks
* Timer-driven preemption
* User-space ready and waiting queues
* Explicit thread states

Supported thread states include:

* `RUNNING`
* `READY`
* `WAITING`
* `EXITED`

This implementation demonstrates lower-level context management by manually configuring saved stack and instruction pointers.

> The `setjmp()` and `longjmp()` implementation depends on Linux, glibc, and architecture-specific behavior.

## Repository Structure

```text
Userland-Threading-Library/
├── one-one/
│   ├── thread.c
│   ├── thread.h
│   ├── tests/
│   ├── Makefile
│   └── test.sh
├── many-one/
│   ├── thread.c
│   ├── thread.h
│   ├── queue.c
│   ├── lock.c
│   ├── tests/
│   ├── Makefile
│   └── test.sh
├── many-one-setjmp-longjmp/
│   ├── thread.c
│   ├── thread.h
│   ├── queue.c
│   ├── tests/
│   ├── Makefile
│   └── test.sh
└── try/
    ├── clone.c
    ├── context.c
    └── futex.c
```

The `try` directory contains smaller experiments involving Linux thread creation, execution contexts, and futex synchronization.

## Public API

The library provides a pthread-inspired interface.

### Thread Management

```c
int thread_create(
    thread_t *thread,
    void *(*start_routine)(void *),
    void *arg
);

void thread_exit(void *retval);

int thread_join(
    thread_t thread,
    void **retval
);

int thread_kill(
    thread_t thread,
    int signal
);
```

### Synchronization

```c
void thread_spin_init(spinlock *lock);
void thread_spin_lock(spinlock *lock);
void thread_spin_unlock(spinlock *lock);

void thread_mutex_init(mutex *lock);
void thread_mutex_lock(mutex *lock);
void thread_mutex_unlock(mutex *lock);
```

The original `many-one` implementation must be initialized before threads are created:

```c
init_threads();
```

## Requirements

* Linux
* GCC
* GNU Make
* Bash
* glibc development headers

Install the required build tools on Ubuntu or Debian:

```bash
sudo apt update
sudo apt install build-essential
```

## Getting Started

Clone the repository:

```bash
git clone https://github.com/vasvi1203/Userland-Threading-Library.git
cd Userland-Threading-Library
```

### One-to-One Implementation

```bash
cd one-one
make
make run
```

### Many-to-One `ucontext` Implementation

```bash
cd many-one
make
make run
```

### Many-to-One `setjmp()`/`longjmp()` Implementation

```bash
cd many-one-setjmp-longjmp
make
make run
```

Clean generated files:

```bash
make clean
```

## Usage Example

The following example creates a thread, passes an integer to it, and retrieves the result using `thread_join()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

void *square(void *arg)
{
    int *result = malloc(sizeof(int));

    if (result == NULL) {
        return NULL;
    }

    int value = *(int *)arg;
    *result = value * value;

    thread_exit(result);
    return NULL;
}

int main(void)
{
    thread_t worker;
    int input = 12;
    void *return_value = NULL;

    if (thread_create(&worker, square, &input) != 0) {
        fprintf(stderr, "Unable to create thread\n");
        return EXIT_FAILURE;
    }

    if (thread_join(worker, &return_value) != 0) {
        fprintf(stderr, "Unable to join thread\n");
        return EXIT_FAILURE;
    }

    if (return_value != NULL) {
        printf("Square of %d = %d\n", input, *(int *)return_value);
        free(return_value);
    }

    return EXIT_SUCCESS;
}
```

Compile the example with the `setjmp()`/`longjmp()` implementation:

```bash
gcc -g example.c thread.c queue.c -o example
./example
```

When using the original many-to-one implementation, call `init_threads()` near the beginning of `main()`.

## Scheduling

The many-to-one implementations maintain a queue of runnable thread control blocks.

Each thread control block stores information such as:

* Thread identifier
* Execution context
* Stack
* Start routine and arguments
* Return value
* Execution status
* Pending signals

A virtual interval timer generates periodic `SIGVTALRM` signals.

When the timer fires, the scheduler:

1. Saves the currently running thread's context.
2. Places the thread back into the ready queue if it is still runnable.
3. Selects the next runnable thread.
4. Restores the selected thread's execution context.
5. Delivers pending signals.
6. Continues execution from the point where the thread was paused.

This creates preemptive, round-robin-style scheduling entirely in user space.

## Synchronization

### Spinlocks

Spinlocks use atomic operations to acquire a lock.

A waiting thread repeatedly checks whether the lock is available. Spinlocks are useful for short critical sections but consume CPU resources while waiting.

### Mutexes

Mutexes prevent multiple threads from accessing shared resources simultaneously.

* The one-to-one implementation uses Linux futexes to block and wake kernel threads.
* The many-to-one implementations use user-space wait queues so that another runnable thread can execute while a thread is waiting.

## Testing

The repository contains tests for:

* Valid and invalid thread creation
* Thread joining and return values
* Thread termination
* Signal delivery
* Spinlocks
* Mutexes
* Concurrent shared-state updates
* Matrix operations
* Large thread counts
* Stress and correctness scenarios

Run the test suite from an implementation directory:

```bash
make
make run
```

Individual generated test executables may also be run directly:

```bash
./create
./join
./sweet
./spin
./mutex
./signal
./matrix
./stress
./stress2
```

Available executables may vary between implementations.

## Concepts Demonstrated

This project demonstrates several operating-system and systems-programming concepts:

* User-level and kernel-level threading
* Thread control blocks
* Context switching
* Preemptive scheduling
* Signal handling
* Stack management
* Atomic operations
* Mutual exclusion
* Futex-based synchronization
* Ready and waiting queues
* Linux process and thread internals

## Limitations

The library is intended for educational use and should not replace POSIX Threads in production applications.

Current limitations include:

* Fixed thread and stack limits
* Simplified resource cleanup
* Limited error handling
* Linux-specific implementation details
* Architecture-specific context manipulation
* A subset of pthread functionality
* No priority-based scheduling
* No condition variables or semaphores
* Limited portability across operating systems and C libraries

## Future Improvements

Potential enhancements include:

* Configurable stack sizes
* Priority-based scheduling
* Additional scheduling algorithms
* Condition variables
* Semaphores
* Deadlock detection
* Thread-local storage
* Improved lifecycle and memory management
* Portable architecture abstractions
* Performance comparisons with POSIX Threads
* Continuous integration for automated tests
