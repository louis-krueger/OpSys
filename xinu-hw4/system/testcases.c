/**
 * Written by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by: by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 *
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("This is the modified test cases!\r\n");
    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d", currpid);
	kprintf(" ...That's a process\n\r");
        /* Uncomment the resched() line for cooperative scheduling. */
        if (1)
	{
	     kprintf("rescheduling...\n\r");
	     resched();
	}
    }
    kprintf("This process %d is done running.\r\n", currpid);
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Process [%d] starting \n\rTesting bigargs...\r\n", currpid); 
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - a = 0x%08X\r\n", currpid, a);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - b = 0x%08X\r\n", currpid, b);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - c = 0x%08X\r\n", currpid, c);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - d = 0x%08X\r\n", currpid, d);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - e = 0x%08X\r\n", currpid, e);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - f = 0x%08X\r\n", currpid, f);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - g = 0x%08X\r\n", currpid, g);
	     kprintf("rescheduling...\n\r");
	     resched();
    kprintf("Process [%d] - h = 0x%08X\r\n", currpid, h);
	     kprintf("rescheduling...\n\r");
	     resched();
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("\nProcess name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack pointer of process  : 0x%08X \r\n",
            ppcb->regs[PREG_SP]);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c, pid;

    kprintf("0) Test creation of one process\r\n");
    kprintf("1) Test passing of many args\r\n");
    kprintf("2) Test passing many args to many processes, party mode\r\n");
    kprintf("3) Create three processes and run them with rescheduling\r\n");

    kprintf("===Starting processes===\r\n");

    c = kgetc();
    switch (c)
    {
    case '0':
        pid = create((void *)main, INITSTK, "MAIN1", 2, 0, NULL);
        printpcb(pid);
        break;

    case '1':
        pid = create((void *)testbigargs, INITSTK, "MAIN1", 8,
                     0x11111111, 0x22222222, 0x33333333, 0x44444444,
                     0x55555555, 0x66666666, 0x77777777, 0x88888888);
        printpcb(pid);
        ready(pid, 0);
        break;

    case '2':
        // create many  programs with many args and have a party
        kprintf("\r\n***PARTY MODE***\r\n"); 
	ready(create((void *)testbigargs, INITSTK, "MAIN1337", 8,
                    0x13371111, 0x22222222, 0x31337333, 0x44444444,
                    0x55555555, 0x66666666, 0x71337777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN2", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x11111111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
	ready(create((void *)testbigargs, INITSTK, "MAIN3", 8,
                    0x22221111, 0x22222222, 0x33333333, 0x44444444,
                    0x55555555, 0x66666666, 0x77777777, 0x88888888), 0);
        while (numproc > 1)
            resched();
        kprintf("\r\n\r\nThat party seemed oddly percise (O.o)\r\n\r\n");
	break;

    
    default:
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL), 0);
        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL), 0);
        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL), 0);
        while (numproc > 1)
            resched();
        break;
    }

    kprintf("\r\n===Done starting processes===\r\n");
    return;
}
