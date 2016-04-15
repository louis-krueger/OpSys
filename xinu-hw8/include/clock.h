/**
 * @file clock.h
 * Definitions relating to the hardware clock and the XINU clock interrupt.
 *
 * $Id: clock.h 307 2007-10-11 20:56:25Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <queue.h>

#define QUANTUM        10     /**< clock ticks until preemption        */

extern ulong preempt;

/**
 * \defgroup clock Real-time Clock
 *
 * Real-time clock functions maintain globally readable clocktime in
 * seconds and a millisecond granularity counter, ctr_ms.  
 *
 *@{
 */
extern ulong ctr_mS;          /**< millisecond counter                 */
extern ulong clocktime;       /**< current time in secs since boot     */
/**@}*/

extern ulong time_base_freq;  /**< frequency of hardware clock update  */
extern ulong time_intr_freq;  /**< frequency of XINU clock interrupt   */

extern queue sleepq;

/* Clock function prototypes */
extern void clockinit(void);
extern void clock_update(ulong);
extern interrupt clockintr(void);
extern short deltainsert(short, queue, short);

#endif                          /* _CLOCK_H_ */
