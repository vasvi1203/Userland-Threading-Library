#include "lock.h"
#include "thread.h"
#include <signal.h>

int xchg(int *addr, int newval)
{
  int result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}

void thread_spin_init(spinlock* spin_lock){
  spin_lock->islocked = 0;
}

// @credit:- xv6 code
void thread_spin_lock(spinlock* spin_lock){
  // The xchg is atomic.
  while(xchg(&(spin_lock->islocked), 1) != 0)
    ;
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
  __sync_synchronize();

}
// @credit:- xv6 code
void thread_spin_unlock(spinlock* spin_lock){
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
   __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (spin_lock->islocked) : );
}

