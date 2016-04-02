/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * Modified by:
 * Samuel Scheel
 * and
 * Louis Krueger
 */
/* Embedded XINU, Copyright (C) 2010, 2014.  All rights reserved. */

#include <xinu.h>
void printFreeMem(void);

void printFreeMem()
{
   memblk* freemem = freelist.next;
   do
   {
	kprintf("current: 0x%08X\r\n", freemem);	
	kprintf("\tnext: 0x%08X\r\n", freemem->next);	
	kprintf("\tlength: 0x%08X\r\n", freemem->length);
	if ((freemem->next != NULL) && (freemem->next != freemem))
		freemem = freemem->next;
	else
		break;
   }
   while(1);
   //return 0;
}

ulong rand(void)
{
    ulong a = 1664525UL;
    ulong b = 1013904223UL;
    static ulong c = 0;

    c = a * c + b;
    return c;
}

syscall sleep(int time)
{
    /* Dumbest sleep ever. */
    int i = 0, j = 0;
    for (i = 0; i < time; i++)
        for (j = 0; j < 1000; j++)
            ;
    return 0;
}

void printpid(int times)
{
	int i = 0;
	//enable
	for (i = 0; i < times; i++)
	{
		resched();
		kprintf("This is process %d - [%s]\r\n", currpid, proctab[currpid].name);
	}
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    
    kprintf("q) Test simple queue\r\n");
    kprintf("1) Print Free memory \r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
	case 'q':
        kprintf("Testing scheduling case: %c\r\n", c);
        //printqueue(readylist);
	ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5),  0);
        //printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 10, "PRINTER-C", 1, 5), 0);
        //printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 15, "PRINTER-B", 1, 5), 0);
        //printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 20, "PRINTER-A", 1, 5), 0);
        //printqueue(readylist);
        kprintf("end of test scheduling\r\n");
	break;
	
	case '1':
	kprintf("***Start Print***\r\n");
	printFreeMem();
	kprintf("*** End  Print***\r\n\n");

	void* p1 = getmem(0);
	kprintf("Requested 0 bytes.\r\n");

	kprintf("***Start Print***\r\n");
        printFreeMem();
        kprintf("*** End  Print***\r\n\n");

	void* p2 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");

	kprintf("***Start Print***\r\n");
        printFreeMem();
        kprintf("*** End  Print***\r\n\n");

	freemem(p1 , 0);
        kprintf("Freed 0 bytes.\r\n");

        kprintf("***Start Print***\r\n");
        printFreeMem();
        kprintf("*** End  Print***\r\n\n");
	break;

   	default:
	kprintf("\r\nNow you've done it\r\n");
        break;
    }

    while (numproc > 2)
    {
        resched();
    }
    kprintf("\r\n===TEST END===\r\n");
    return;
}
