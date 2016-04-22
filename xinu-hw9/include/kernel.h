/**
 * @file kernel.h 
 *
 * The base include file for the XINU kernel. Defines symbolic constants,
 * universal return constants, intialization constants, machine size
 * definitions, inline utility functions, and include types
 *
 * $Id: kernel.h 180 2007-07-12 14:15:16Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef _KERNEL_H_
#define _KERNEL_H_

/* Symbolic constants used throughout Xinu */

/* Base type definitions */
typedef unsigned char uchar;     /**< unsigned char type                  */
typedef unsigned short ushort;   /**< unsigned short type                 */
typedef unsigned long ulong;     /**< unsigned long type                  */
typedef unsigned long irqmask;   /**< machine status for disable/restore  */

/* Boolean type and constants */
typedef char bool;               /**< boolean type                         */
#define	FALSE        0           /**< boolean false                        */
#define	TRUE         1           /**< boolean true                         */

/* Null constants */
#define	NULL            0       /**< null pointer for linked lists        */
#define EMPTY         (-1)      /**< null pointer for queues              */

/* Function declaration return types */
#define	syscall	    long        /**< system call declaration              */
#define	devcall     long        /**< device call declaration              */
#define	local       static long /**< local procedure declaration          */
#define	command     long        /**< shell command declaration            */
#define	process     long        /**< process declaration                  */
#define	interrupt   void        /**< interrupt procedure                  */
#define message     int         /**< message passing data                 */

/* Universal return constants */
#define	OK        1             /**< system call ok                       */
#define	SYSERR   (-1)           /**< system call failed                   */
#define EOF      (-2)           /**< End-of-file (usually from read)      */
#define TIMEOUT  (-3)           /**< system call timed out                */
#define NOMSG    (-4)           /**< no message received                  */

/* Reschedule constants for ready  */
#define	RESCHED_YES 1           /**< tell ready to reschedule             */
#define	RESCHED_NO  0           /**< tell ready not to reschedule         */

/* Configuration information */
#define RTCLOCK   TRUE          /**< now have RTC support                 */
#define PREEMPT   TRUE          /**< preemptive rescheduling              */
#define AGING     TRUE         /**< queue aging prevents starvation      */
#define NPROC     50            /**< number of user processes             */
#define NSEM      100           /**< number of semaphores                 */

/* Assertions */
#define ASSERT(cond)              if ( !(cond) ) return SYSERR
#define ASSERTFUNC(func_call)     ASSERT((func_call) == OK)

/* Kernel function prototypes */
int nulluser(void);

syscall kprintf(char *fmt, ...);

irqmask disable(void);
irqmask restore(irqmask);
irqmask enable(void);
irqmask enable_irq(irqmask);

/**
 * \defgroup process Process Management API
 * @{
 */
syscall create(void *, ulong, ulong, char *, ulong, ...);
syscall getpid(void);
syscall kill(int);
syscall ready(long, bool);
syscall resched(void);
syscall sleep(int n);
/**@}*/
syscall unsleep(int pid);

/**
 * \defgroup message Inter-Process Communication prototypes
 * @{
 */
syscall send(int, message);
message receive(void);
message recvclr(void);
message recvtime(int);
/**@}*/

#endif                          /* _KERNEL_H_ */
