#include "types.h"
#include "user.h"
#include "xthread.h"


void xthread_create(int * tid, void * (* start_routine)(void *), void * arg)
{
    void *stack = malloc(4096);
    *tid = clone(start_routine, stack, arg);
}


void xthread_exit(void * ret_val_p)
{
    thread_exit(ret_val_p);
}


void xthread_join(int tid, void ** retval)
{
    void* stack;
    join(tid, retval, &stack);
    free(stack);

}
