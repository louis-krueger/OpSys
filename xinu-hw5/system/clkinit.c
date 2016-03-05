/**
 * @file     clkinit.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/*
 * Number of timer interrupts that have occurred since clktime was
 * incremented.  When clkticks reaches CLKTICKS_PER_SEC, clktime is
 * incremented again and clkticks is reset to 0.
 */
volatile ulong clkticks;

/* Number of seconds that have elapsed since the system booted.  */
volatile ulong clktime;

#if PREEMPT
/* preemption counter                                            */
volatile ulong preempt;
#endif


/**
 * Initialize the clock.  This function is called at startup.
 */
void clkinit(void)
{
#if PREEMPT
    preempt = QUANTUM;          /* initial time quantum      */
#endif
    clkticks = 0;

    kprintf("Time base %dHz, Clock ticks at %dHz\r\n",
            platform.clkfreq, CLKTICKS_PER_SEC);

    /* register clock interrupt */
    interruptVector[IRQ_TIMER] = clkhandler;
    enable_irq(IRQ_TIMER);
    clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);
}
