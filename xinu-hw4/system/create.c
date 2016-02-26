/**
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

#define DEBUG 2    /** comment out definition to stop printing debug info  **/

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
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
 
#ifdef DEBUG
	kprintf("\n\n***DEBUG INFO START (create.c) before pcb setup***\n\r");
	if (DEBUG > 0)
		kprintf("process name:%s\n\r", ppcb->name);
	
	if (DEBUG > 1)
	{
		kprintf("proc state:%d\n\r", ppcb->state);    //ppcb->state same thing as (*ppcb).state
		kprintf("stack base address:0x%08X\n\r", ppcb->stkbase);
		kprintf("stack length:0x%08X\n\r", ppcb->stklen);
		kprintf("***DEBUG INFO END***\n\n\r");
	}
#endif


   
    // TODO: Setup PCB entry for new process.
	//1.PCB state set has been provided in the line above
    
		/* setup PCB entry for new proc */
   	 ppcb->state = PRSUSP;

	//2.need to set new proc pointer to the run time stack
		
		/*stack base pointer NOTE:&saddr? Might be the other stack i didn't check  print will tell */
	ppcb->stkbase = saddr;
			
	//3.need to set stklen 
		
		/*set stack size to ssize */		
	ppcb->stklen = ssize; 
	
	//4.need to set process name
	
		/*NOTE:need to get full name from attribs */
	ppcb->name = name;
	
	//5.need to store process registers
	
		/*rough sketch*/
	ppcb->*regs =  funcaddr;

//This is the definition of the pcb, this comment is also at the top.
	
/*
 *    		int state;                  **< process state: PRCURR, etc.             *
 *    		void *stkbase;              **< base of run time stack                  *
 *    		int stklen;                 **< stack length                            *
 *    		char name[PNMLEN];          **< process name                            *
 *    		int regs[PREGS];            **< stored process registers                *
 */

#ifdef DEBUG
	kprintf("\n\n***DEBUG INFO START (create.c) after pcb setup***\n\r");
	if (DEBUG > 0)
		kprintf("process name:%s\n\r", ppcb->name);
	
	if (DEBUG > 1)
	{
		kprintf("proc state:%d\n\r", ppcb->state);    //ppcb->state same thing as (*ppcb).state
		kprintf("stack base address:0x%08X\n\r", ppcb->stkbase);
		kprintf("stack length:0x%08X\n\r", ppcb->stklen);
		kprintf("***DEBUG INFO END***\n\n\r");
	}
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
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

    // TODO: Initialize process context.
    //
    // TODO:  Place arguments into activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.

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
