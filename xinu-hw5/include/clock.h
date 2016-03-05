/**
 * @file clock.h
 * Definitions relating to the hardware clock and the Xinu clock interrupt.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#ifndef _CLOCK_H_
#define _CLOCK_H_

/**
 * Timer resolution.  This refers to the maximum number of times that Xinu
 * schedules the timer interrupt per second, @a not to the number of native
 * clock cycles per second.
 */
#define CLKTICKS_PER_SEC  1000
#define QUANTUM        10       /* clock ticks until preemption        */

extern volatile ulong preempt;
extern volatile ulong clkticks;
extern volatile ulong clktime;

/* Clock function prototypes.  Note:  clkupdate() and clkcount() are documented
 * here because their implementations are platform-dependent.  */

void clkinit(void);

/**
 * Sets up a timer interrupt to trigger after a certain number of clock cycles
 * have elapsed.
 *
 * @param cycles
 *     Number of cycles after which the timer interrupt is to be triggered.
 *     This refers to native clock cycles (whose frequency is specified in
 *     clkfreq).
 */
void clkupdate(ulong cycles);

/**
 * Gets the current system timer cycles.
 *
 * @return
 *	The current number of timer cycles.  Generally, only the difference between
 *	the value returned by between two successive calls to this function is
 *	meaningful.  The number of cycles that elapse per second is specified by the
 *	@ref platform::clkfreq "value in platform".
 */
ulong clkcount(void);

interrupt clkhandler(void);
void udelay(ulong);
void mdelay(ulong);

#endif                          /* _CLOCK_H_ */
