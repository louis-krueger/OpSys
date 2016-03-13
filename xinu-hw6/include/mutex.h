/**
 * @file mutex.h
 *
 */
/* Embedded Xinu, Copyright (C) 2010.  All rights reserved. */

#ifndef _MUTEX_H_
#define _MUTEX_H_

/* Mutex function prototypes */
void mutexInit(void);
void mutexAcquire(void);
void mutexRelease(void);
bool testAndSet(volatile bool *);

#endif                          /* _MUTEX_H */
