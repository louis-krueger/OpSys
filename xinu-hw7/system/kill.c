/**
 * @file kill.c
 * Provides: kill
 *
 * COSC 3250/ COEN 4820 Assignment 4
 */

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/*
 * kill  --  kill a process and remove it from the system
 */
syscall kill(int pid)
{
    irqmask im;
    pcb *ppcb;                  /* points to process control block for pid */

    im = disable();
    if (isbadpid(pid) || (0 == pid)
        || (PRFREE == (ppcb = &proctab[pid])->state))
    {
        restore(im);
        return SYSERR;
    }

    ppcb = &proctab[pid];

    --numproc;

    switch (ppcb->state)
    {
    case PRCURR:
        ppcb->state = PRFREE;   /* suicide */
        resched();

    case PRREADY:
        remove(pid);

    default:
        ppcb->state = PRFREE;
    }

    restore(im);
    return OK;
}
