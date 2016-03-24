/**
 * @file semfree.c
 * @provides semfree.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Deallocate a semaphore.
 * Reset the semaphore count, releasing any threads
 * in the waiting queue.  Deallocate entry in global
 * semaphore table.
 * @param sem  target semaphore
 * @return OK on success, SYSERR on failure
 */
syscall semfree(semaphore sem)
{
    register semblk *semptr;
    irqmask im;
    int pid;

    im = disable();
    if (isbadsem(sem))
    {
        restore(im);
        return SYSERR;
    }

    semptr = &semtab[sem];
    while (nonempty(semptr->queue))
    {
        pid = dequeue(semptr->queue);   /* free waiting threads */
        ready(pid, FALSE);
    }

    semptr->count = 0;
    semptr->state = SFREE;
    restore(im);
    return OK;
}
