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
   kprintf("\n\r\r\n ========== \r\n");
   kprintf("***Start Print***\r\n");
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
   kprintf("***End Print***\r\n");
   //return 0;
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    
    kprintf("1) Get-Free Test \r\n");
    kprintf("2) Get-Free-Get Test1 \r\n");
    kprintf("3) Get-Free-Get Test2 \r\n");
    kprintf("4) Get-Full Get Test \r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
	case '1':
	printFreeMem();

	void* p1 = getmem(0);
	kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

	void* p2 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p3 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p4 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p5 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p6 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        printFreeMem();

	freemem(p1, 0);
	kprintf("Freed 0 bytes.\r\n");
        freemem(p3, 16);
	kprintf("Freed 16 bytes.\r\n");
	freemem(p5, 16);
        kprintf("Freed 16 bytes.\r\n");

        printFreeMem();
	break;
	
	case '2':
        printFreeMem();

        void* p11 = getmem(0);
        kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

        void* p12 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
	
        printFreeMem();

        void* p13 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        void* p14 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
        
	freemem(p12, 16);
        kprintf("Freed p12.\r\n");
	
	freemem(p14, 16);
        kprintf("Freed p14.\r\n");
        
        printFreeMem();
        void* p00 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
       	break;

	case '3':
        printFreeMem();

        void* p21 = getmem(0);
        kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

        void* p22 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");

        printFreeMem();

        void* p23 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        void* p24 = getmem(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        freemem(p22, 16);
        kprintf("Freed p22.\r\n");

        freemem(p24, 16);
        kprintf("Freed p24.\r\n");

        printFreeMem();
        void* p20 = getmem(8);
        kprintf("Requested 8 bytes.\r\n");
        printFreeMem();
        break;

	
	case '4':
        printFreeMem();

        void* p31 = getmem(0);
        kprintf("Requested 0 bytes.\r\n");

	void* p41 = getmem(roundmb(0x07FFFFF7 - (ulong)p31));
	kprintf("Requested 0x%08X  bytes\r\n", roundmb(0x07FFFFF7 - (ulong)p31));

	printFreeMem();
	
	freemem(p41, roundmb(0x07FFFFF7 - (ulong)p31));
	printFreeMem();
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
