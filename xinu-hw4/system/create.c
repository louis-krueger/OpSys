/**
 * Written by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/*
 *
 *	typedef struct pentry
 *	{	
 *    		int state;                  **< process state: PRCURR, etc.             *
 *    		void *stkbase;              **< base of run time stack                  *
 *    		int stklen;                 **< stack length                            *
 *    		char name[PNMLEN];          **< process name                            *
 *    		int regs[PREGS];            **< stored process registers                *
 *	} pcb;
 *
 *
 *	Definition of pcb from proc.h header file from include\
 *
 *
 *
 *	***DEFINITION OF SOME IMPORTANT THINGS***
 *
 * unusual value marks the top of the process stack                      *
 *#define STACKMAGIC 0x0A0AAA99
 *
 * process state constants                                               *
 *
 *#define PRFREE      0       **< process slot is free                    *
 *#define PRCURR      1       **< process is currently running            *
 *#define PRSUSP      2       **< process is suspended                    *
 *#define PRREADY     3       **< process is on ready queue               *
 *
 * miscellaneous process definitions                                     *
 *
 *#define PNMLEN      16      **< length of process "name"                *
 *
 * the null process is always eligible to run                            *
 *#define NULLPROC    0       **< id of the null process                  *
 *#define BADPID     (-1)     **< used when invalid pid needed            *
 *
 */





/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */

//#define DEBUG 3    /** comment out definition to stop printing debug info  **/

syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    void INITRET(void);

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
#ifdef DEBUG
	if (DEBUG > 1)
	{
		kprintf("saddr address - [0x%08X]\n\r", saddr);
	}
#endif
    
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }
    numproc++;
    ppcb = &proctab[pid];
 
#ifdef DEBUG
	if (DEBUG > 4)
	{	
		kprintf("\n\n***DEBUG INFO START (create.c) before pcb setup***\n\r");
		kprintf("process name:%s\n\r", ppcb->name);
		kprintf("proc state:%d\n\r", ppcb->state);    //ppcb->state same thing as (*ppcb).state
		kprintf("stack base address:0x%08X\n\r", ppcb->stkbase);
		kprintf("stack length:0x%08X\n\r", ppcb->stklen);
	}
	kprintf("***DEBUG INFO END***\n\n\r");
#endif
  
   	// TODO: Setup PCB entry for new process.
	
	// Set PCB state to SUSPENDED 
   	ppcb->state = PRSUSP;
	
	// Set PCB stack base to the address of saddr minus its length
	ppcb->stkbase = (ulong *)((ulong)saddr - ssize);
	
	// Set PCB stack length to the address of saddr minus the stack's base
	ppcb->stklen = ((ulong)saddr) - (ulong)ppcb->stkbase;
	
	// Set PCB name to the argument above
	strncpy(ppcb->name, &name[0], PNMLEN);
	

#ifdef DEBUG
	kprintf("\n\n***DEBUG INFO START (create.c) after pcb setup***\n\r");
	if (DEBUG > 0)
		kprintf("process name:%s\n\r", ppcb->name);
	
	if (DEBUG > 1)
	{
		kprintf("proc state:%d\n\r", ppcb->state);    //ppcb->state same thing as (*ppcb).state
		kprintf("stack base address:0x%08X\n\r", ppcb->stkbase);
		kprintf("stack length:0x%08X\n\r", ppcb->stklen);
	}
	kprintf("***DEBUG INFO END***\n\n\r");
#endif

    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 4) * 4;
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */

#ifdef DEBUG 
	if(DEBUG > 2)
	{
		kprintf("size of ppcb: (0x%08X) - %lo\n\r", sizeof(*ppcb), sizeof(*ppcb));
		kprintf("pads value: [0x%08X] - %d\n\r", pads, pads); 
		kprintf("nargs value: [0x%08X] - %d\n\n\r", nargs, nargs);
	}
#endif    

	/*Inspired by TA bot */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }
    for (i = 0; i < PREGS; i++)
    {
        ppcb->regs[i] = 0;
    }
    // TODO: Initialize process context.
    ppcb->regs[PREG_PC] = (int)funcaddr;
    ppcb->regs[PREG_LR] = (int)INITRET;
    ppcb->regs[PREG_SP] = (int)saddr;
    // TODO:  Place arguments into activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.
    
	/* if greater than 4 inspired by TA bot */ 

    va_start(ap, nargs);
    for (i = 0; i < nargs; i++)
    {
	if (i < 4)
            ppcb->regs[i] = va_arg(ap, ulong);
        else
            saddr[i - 4] = va_arg(ap, ulong);
    }
    va_end(ap);

#ifdef DEBUG
    if(DEBUG > 0)
    {
	int k = 0;
	ulong *magicStack = saddr;
	do
	{
		kprintf("[0x%08X] value(*| ):  0x%08X (0x%08X)\n\r", magicStack, *magicStack, *magicStack);	
		k++;
		magicStack++;
	}
	while(k < pads + nargs  + 5);	
    }

#endif
     
    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kill(currpid);
}
