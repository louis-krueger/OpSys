/**
 * @file wait.c
 * @provides wait.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Make current process wait on a semaphore
 * @param sem  target semaphore
 * @return OK on success, SYSERR on failure
 */
syscall wait(semaphore sem)
{
    register semblk *semptr = NULL;
    register pcb *ppcb = NULL;
    irqmask im;

    im = disable();
    if (isbadsem(sem))
    {
        restore(im);
        return SYSERR;
    }
    ppcb = &proctab[currpid];
    semptr = &semtab[sem];
    if (--(semptr->count) < 0)
    {
        ppcb->state = PRWAIT;
        ppcb->sem = sem;
        enqueue(currpid, semptr->queue);
        resched();
    }
    restore(im);
    return OK;
}
