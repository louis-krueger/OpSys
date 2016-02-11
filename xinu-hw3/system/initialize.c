/**
 * @file initialize.c
 * @provides nulluser, sysinit
 *
 * $Id: initialize.c 191 2007-07-13 22:28:23Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/** 
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null process remains always in
 * a ready (PRREADY) or running (PRCURR) state.
 */

#include <kernel.h>
#include <platform.h>
#include <device.h>
#include <string.h>
#include <mips.h>

/* Linker provides start and end of image */
extern   void   _start(void);   /* start of XINU code                    */    
extern   void   *end;           /* end of XINU code                      */

/* Function prototypes */
local       platforminit(void); /* determines platform-dependent settings*/
local       sysinit(void);      /* intializes system structures          */
void        testcases(void);	/* testing hook for your testcases       */

/* Params set by startup.S */
void            *minheap;       /* Bottom of heap (top of O/S stack)     */
ulong           cpuid;          /* Processor id                          */

struct platform platform;       /* Platform specific configuration       */

/**
 * Intializes the system and becomes the null process.
 * This is where the system begins after the C environment has been 
 * established.  Interrupts are initially DISABLED, and must eventually 
 * be enabled explicitly.  This routine turns itself into the null process 
 * after initialization.  Because the null process must always remain ready 
 * to run, it cannot execute code that might cause it to be suspended, wait 
 * for a semaphore, or put to sleep, or exit.  In particular, it must not 
 * do I/O unless it uses kprintf for synchronous output.
 */
int nulluser()
{
	kprintf(VERSION);  kprintf("\r\n\r\n");
	
	platforminit();

#ifdef DETAIL
	/* Output detected platform. */
	kprintf("Processor identification: 0x%08X\r\n", cpuid);
	kprintf("Detected platform as: %s\r\n\r\n",platform.name);
#endif

	sysinit();

	/* Output XINU memory layout */
	kprintf("%10d bytes physical memory.\r\n", 
		(ulong) platform.maxaddr & 0x7FFFFFFF );
#ifdef DETAIL
	kprintf("           [0x%08X to 0x%08X]\r\n",
		(ulong) KSEG0_BASE, (ulong) (platform.maxaddr - 1));
#endif 
	kprintf("%10d bytes reserved system area.\r\n",
		(ulong) _start - KSEG0_BASE);
#ifdef DETAIL
	kprintf("           [0x%08X to 0x%08X]\r\n",
		(ulong) KSEG0_BASE, (ulong) _start - 1);
#endif 

	kprintf("%10d bytes XINU code.\r\n",
		(ulong) &end - (ulong) _start);
#ifdef DETAIL
	kprintf("           [0x%08X to 0x%08X]\r\n",
		(ulong) _start, (ulong) &end - 1);
#endif 

	kprintf("%10d bytes stack space.\r\n",
		(ulong) minheap - (ulong) &end);
#ifdef DETAIL
	kprintf("           [0x%08X to 0x%08X]\r\n",
		(ulong) &end, (ulong) minheap - 1);
#endif 

	kprintf("%10d bytes heap space.\r\n",
		(ulong) platform.maxaddr - (ulong) minheap);
#ifdef DETAIL
	kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
		(ulong) minheap, (ulong) platform.maxaddr - 1);
#endif 

	kprintf("Hello XINU World!\r\n");

	testcases();

	while(1) 
	{
	}
}

/**
 * Intializes all XINU data structures and devices.
 * @return OK if everything is initialized successfully
 */
local sysinit(void)
{
	return OK;
}

/**
 * Determines and stores all platform specific information.
 * @return OK if everything is determined successfully
 */
local platforminit(void)
{
	switch (cpuid & PRID_REV)
	{
		case PRID_REV_WRT54G:
			strncpy(platform.name, "Linksys WRT54G", 16);
			platform.maxaddr = (void *) (KSEG0_BASE | MAXADDR_WRT54G);
			platform.time_base_freq = TIME_BASE_FREQ_WRT54G;
			platform.uart_dll = UART_DLL_WRT54G;
			break;
		case PRID_REV_WRT54GL:
			strncpy(platform.name, "Linksys WRT54GL", 16);
			platform.maxaddr = (void *) (KSEG0_BASE | MAXADDR_WRT54GL);
			platform.time_base_freq = TIME_BASE_FREQ_WRT54GL;
			platform.uart_dll = UART_DLL_WRT54GL;
			break;
		case PRID_REV_WRT350N:
			strncpy(platform.name, "Linksys WRT350N", 16);
			platform.maxaddr = (void *) (KSEG0_BASE | MAXADDR_WRT350N);
			platform.time_base_freq = TIME_BASE_FREQ_WRT350N;
			platform.uart_dll = UART_DLL_WRT350N;
			break;
		default:
			strncpy(platform.name, "Unknown Platform", 16);
			platform.maxaddr = (void *) (KSEG0_BASE | MAXADDR_DEFAULT);
			platform.time_base_freq = TIME_BASE_FREQ_DEFAULT;
			return SYSERR;
	}
	
	return OK;
}

