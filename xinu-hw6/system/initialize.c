/**
 * @file initialize.c
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null process remains always in
 * a ready (PRREADY) or running (PRCURR) state.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
static int sysinit(void);       /* intializes system structures   */
static void print_os_info(void);        /* Print initial O/S data         */
extern void main(void);         /* Main program                   */

/* Declarations of major kernel variables */
pcb proctab[NPROC];             /* Process table                         */
qid_typ readylist;              /* List of READY processes               */

/* Active system status */
int numproc;                    /* Number of live user processes         */
int currpid;                    /* Id of currently running process       */

/* Params set by startup.S */
void *memheap;                  /* Bottom of heap (top of O/S stack)   */
ulong cpuid;                    /* Processor id                        */

struct platform platform;       /* Platform specific configuration     */

/**
 * Intializes the system and becomes the null thread.
 * This is where the system begins after the C environment has been 
 * established.  Interrupts are initially DISABLED, and must eventually 
 * be enabled explicitly.  This routine turns itself into the null thread 
 * after initialization.  Because the null thread must always remain ready 
 * to run, it cannot execute code that might cause it to be suspended, wait 
 * for a semaphore, or put to sleep, or exit.  In particular, it must not 
 * do I/O unless it uses kprintf for synchronous output.
 */
void nulluser(void)
{
    /* Platform-specific initialization  */
    platforminit();

    /* General initialization  */
    sysinit();

    /* Standard Embedded Xinu processor and memory info */
    print_os_info();

    /* Call the main program */
    ready(create((void *)main, INITSTK, 1, "MAIN", 2, 0, NULL), 0);
    /* enable interrupts here */
    enable();

    /* null process has nothing else to do but cannot exit  */
    while (1)
    {
        if (nonempty(readylist))
            resched();
        /* If there are no processes left in the system, completed. */
        if (numproc <= 1)
        {
            kprintf("\r\n\r\nAll user processes have completed.\r\n\r\n");
            while (1)
                ;
        }
    }

}

static void print_os_info(void)
{
    kprintf(VERSION);
    kprintf("\r\n\r\n");

    /* Output detected platform. */
    kprintf("Processor identification: 0x%08X\r\n", cpuid);
    kprintf("Detected platform as: %s, %s\r\n\r\n",
            platform.family, platform.name);

    /* Output Xinu memory layout */
    kprintf("%10d bytes physical memory.\r\n",
            (ulong)platform.maxaddr - (ulong)platform.minaddr);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)(platform.maxaddr - 1));


    /* Start of kernel in memory (provided by linker)  */
    extern void _start(void);
    kprintf("%10d bytes reserved system area.\r\n",
            (ulong)_start - (ulong)platform.minaddr);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)_start - 1);

    kprintf("%10d bytes Xinu code.\r\n", (ulong)&_end - (ulong)_start);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)_start, (ulong)&_end - 1);

    kprintf("%10d bytes stack space.\r\n", (ulong)memheap - (ulong)&_end);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)&_end, (ulong)memheap - 1);

    kprintf("%10d bytes heap space.\r\n",
            (ulong)platform.maxaddr - (ulong)memheap);
    kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
            (ulong)memheap, (ulong)platform.maxaddr - 1);
    kprintf("\r\n");
}

/**
 * Intializes all Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
static int sysinit(void)
{
    int i = 0;
    pcb *ppcb = NULL;           /* process control block pointer */
    semblk *psem = NULL;        /* semaphore block pointer       */

    /* Initialize system variables */
    /* Count this NULLPROC as the first process in the system. */
    numproc = 1;

    /* Initialize process table */
    for (i = 0; i < NPROC; i++)
    {
        proctab[i].state = PRFREE;
    }

    /* initialize null process entry */
    ppcb = &proctab[NULLPROC];
    ppcb->state = PRCURR;
    strncpy(ppcb->name, "prnull", 7);
    ppcb->stkbase = (void *)&_end;
    ppcb->regs[PREG_SP] = NULL;
    ppcb->stklen = (ulong)memheap - (ulong)&_end;
    ppcb->priority = 0;
    /* TODO: This line won't compile properly until you have added
     * a semaphore field to the process control block.
     */
    ppcb->sem = EMPTY;
    currpid = NULLPROC;

    /* Initialize semaphores */
    for (i = 0; i < NSEM; i++)
    {
        psem = &semtab[i];
        psem->state = SFREE;
        psem->count = 0;
        psem->queue = newqueue();
    }

    /* initialize bounded-waiting mutex subsystems */
    mutexInit();


    /* initialize process ready list */
    readylist = newqueue();

#if RTCLOCK
    /* clear UART interrupt */
    disable_irq(IRQ_USB);
    disable_irq(IRQ_PCM);
    disable_irq(IRQ_PL011);
    disable_irq(IRQ_SD);
    /* initialize real time clock */
    clkinit();
#endif

    return OK;
}
