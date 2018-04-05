#ifndef _XTHREAD_H
#define _XTHREAD_H

void xthread_create(int * tid, void * (*start_routine)(void *), void * arg);
void xthread_exit(void * ret_val_p);
void xthread_join(int tid, void ** retval);

#endif
