/**
 * @file semcount.c
 * @provides semcount.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Return a semaphore's count
 * @param sem    target semaphore
 * @return count from semtab entry on success, SYSERR on failure
 */
syscall semcount(semaphore sem)
{
    if (isbadsem(sem))
    {
        return SYSERR;
    }

    return (semtab[sem].count);
}
