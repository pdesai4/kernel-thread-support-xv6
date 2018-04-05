**CS 550-04 Operating Systems, Spring 2017**

Add kernel thread support and implement a small thread library for xv6 OS.

This project is about adding kernel support for thread management in xv6 and implementing a small thread library (called xthread) based on the new kernel feature.

When user calls xthread_create(), a user level stack of one page size is created and this stack is passed to the clone() when it is called. An extra user stack pointer, newstack, is now added to the structure of a process.

In clone() :
	This system call works similar to fork(). The new proocess is allocated by allocproc(). This new process has the same size as of its parent process and shares its parent process's page directory. The trap frame value for the new process is set as:

					newp->tf->eip = (uint)fn;					//The eip points to the adress of the start_routine.
				newp->tf->esp = (uint)(stack+4096-4);		//esp points to (base + size - size of a stack entry)
				*((uint*)(newp->tf->esp)) = (uint)arg;		//The stack gets the argument entry (for start_routine)
				*((uint*)(newp->tf->esp - 4)) = 0xFFFFFFFF;	//This specifies return to nowhere
				newp->tf->esp =(newp->tf->esp) -4;			//esp is now updated
				
		pid of this new thread is returned as a thread id.

When the user calls xthread_exit(), the system call thread_exit() is called.

In thread_exit() :
	This system call works similar to the exit(). The actual exit() is changed in order to not let the exiting thread's children to be adopted by the initproc (since we will be handling this on our own). The thread_exit() before ending assigns the eax register of the terminating process the passed value so that this value is returned to join when called in future.
							`proc->tf->eax = (uint)ret_val_p;`
When the start_routine is returned, a page fault occurs since the return address is set as 0xFFFFFFFF in clone. To handle this page fault, thread_exit() is called from trap.c with the value of the thread's eax register.
When the user calls xthread_join(), the system call join() is called. This function also free the user stack.

In join() :
	This system call works similar to wait(). Some adjustments are made to the wait() function. The wait() before freeing any thread checks whether it has any child thread. If its child thread also shares the page directory as of the current thread, then the current thread has to wait until child is free. After the child thread is free only then can the parent thread be free. The join() puts the return value of the thread to be 			free into pointer `*ret_p` from the thread's eax register and puts the user stack of the thread into pointer `*stack`.
									`*ret_p = (void*)p->tf->eax;`
							            `*stack = (void*)p->newstack;`
							            