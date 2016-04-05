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
void *getMemForTest(uint nbytes);
syscall freeMemForTest(void*);

void printFreeMem()
{
   memblk* freemem = freelist.next;
   kprintf(" ========== \r\n");
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
   kprintf("\tcurrent is the tail of heap, calcuatled end [0x07ffffff] - 0x%08X\r\n", (uint)freemem + (uint)freemem->length - 1);
   kprintf("***End Print***\r\n\r\n");
}
void *getMemForTest(uint nbytes)
{
	kprintf("Requested %d bytes.\r\n", nbytes);
	void* T = malloc(nbytes);
	if (T == (void *)SYSERR)
		kprintf("Not enough space in freelist.\r\n");
	return T;
}
syscall freeMemForTest(void* p)
{
	kprintf("Requested free 0x%08X.\r\n", p);	
	if (free(p) == SYSERR)
		kprintf("Failed to free.\r\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    void* p01;
    void* p02;
    void* p03;
    void* p04;
    void* p05;
    void* p06;
    void* p07;
    void* p08;
    void* p09;
    void* p10;
    void* p11;
    void* p12;
    void* p13;
    void* p14;
    void* p15;
    void* p16;
    void* p17;
    void* p18;
    void* p19;
    void* p20;

    kprintf("0) Simple Test\r\n");
    kprintf("1) Get-Free Test \r\n");
    kprintf("2) Get-Free-Get Test1 \r\n");
    kprintf("3) Get-Free-Get Test2 \r\n");
    kprintf("4) Get-Full Get Test \r\n");
    kprintf("5) Holes in Freelist Test\r\n");
    kprintf("6) Simple Hole Test\r\n");
    kprintf("7) Test 5 twice, check for memory leaks\r\n");
    kprintf("8) Request all the memory\r\n");
    kprintf("9) Request all the memory, and then request it all again\r\n");
    kprintf("a) Free pointer that is not allocated\r\n");
    kprintf("===TEST BEGIN===\r\n");
    // TODO: Test your operating system!
	
    c = kgetc();
    switch (c)
    {
    case '0':
	printFreeMem();
	p01 = getMemForTest(16);
        printFreeMem();
	p02 = getMemForTest(32);
	printFreeMem();
	p03 = getMemForTest(64);
	printFreeMem();
	freeMemForTest(p01);
	printFreeMem();
	freeMemForTest(p02);
        printFreeMem();
	freeMemForTest(p03);
        printFreeMem();
	break;
    
    case '1':
	printFreeMem();
	p01 = getMemForTest(0);
        printFreeMem();
	p02 = getMemForTest(16);
        printFreeMem();
	p03 = getMemForTest(16);
        printFreeMem();
	p04 = getMemForTest(16);
        printFreeMem();
	p05 = getMemForTest(16);
        printFreeMem();
	p06 = getMemForTest(16);
        printFreeMem();
	freeMemForTest(p01);
        freeMemForTest(p03);
	freeMemForTest(p05);
        printFreeMem();
	freeMemForTest(p02);
	freeMemForTest(p04);
	freeMemForTest(p06);
	printFreeMem();
	break;
	
    case '2':
        printFreeMem();
        p01 = getMemForTest(0);
        printFreeMem();
        p02 = getMemForTest(16);
        printFreeMem();
        p03 = getMemForTest(16);
        printFreeMem();
        p04 = getMemForTest(16);
        printFreeMem();
	freeMemForTest(p02);
	freeMemForTest(p04);
        printFreeMem();
        p05 = getMemForTest(16);
        printFreeMem();
       	freeMemForTest(p01);
	freeMemForTest(p03);
	freeMemForTest(p05);
	printFreeMem();
	break;

    case '3':
        printFreeMem();
        p01 = getMemForTest(0);
        printFreeMem();
        p02 = getMemForTest(16);
        printFreeMem();
        p03 = getMemForTest(16);
        printFreeMem();
        p04 = getMemForTest(16);
        printFreeMem();
        freeMemForTest(p02);
        freeMemForTest(p04);
        printFreeMem();
        p05 = getMemForTest(8);
        printFreeMem();
        freeMemForTest(p01);
	freeMemForTest(p03);
	freeMemForTest(p05);
	printFreeMem();
	break;

	
    case '4':
        printFreeMem();
        p01 = getMemForTest(8);
	printFreeMem();
	p02 = getMemForTest((uint)roundmb(0x07FFFFEF - (int)p01));
	printFreeMem();	
	freeMemForTest(p02);
	printFreeMem();
	freeMemForTest(p01);
	printFreeMem();
	break;

    case '5':
        printFreeMem();
	p20 = getMemForTest(8);
	p01 = getMemForTest(8);
	p02 = getMemForTest(16);
	p03 = getMemForTest(16);
	p04 = getMemForTest(32);
	p05 = getMemForTest(32);
	p06 = getMemForTest(64);
	p07 = getMemForTest(64);
	p08 = getMemForTest(128);
	p09 = getMemForTest(128);
	p10 = getMemForTest(256);
	p11 = getMemForTest(256);
	p12 = getMemForTest(512);
	p13 = getMemForTest(512);
	p14 = getMemForTest(1024);
	p15 = getMemForTest(1024);
	p16 = getMemForTest(2048);
	p17 = getMemForTest(2048);
	p18 = getMemForTest(4096);
	p19 = getMemForTest(4096);

	printFreeMem();
	freeMemForTest(p20);
	freeMemForTest(p02);
	freeMemForTest(p04);
	freeMemForTest(p06);
	freeMemForTest(p08);
	freeMemForTest(p10);
	freeMemForTest(p12);
	freeMemForTest(p14);
	freeMemForTest(p16);
	freeMemForTest(p18);
	printFreeMem();
	freeMemForTest(p01);
	freeMemForTest(p03);
	freeMemForTest(p05);
	freeMemForTest(p07);
	freeMemForTest(p09);
	freeMemForTest(p11);
	freeMemForTest(p13);
	freeMemForTest(p15);
	freeMemForTest(p17);
	freeMemForTest(p19);	
	printFreeMem();
	break;
    
    case '6':
	printFreeMem();
	p01 = getMemForTest(8);
	printFreeMem();
	p02 = getMemForTest(8);
	printFreeMem();
	freeMemForTest(p01);
	printFreeMem();
	freeMemForTest(p02);
	printFreeMem();
	break;
    
    case '7':
        printFreeMem();
	p20 = getMemForTest(8);
	p01 = getMemForTest(8);
	p02 = getMemForTest(16);
	p03 = getMemForTest(16);
	p04 = getMemForTest(32);
	p05 = getMemForTest(32);
	p06 = getMemForTest(64);
	p07 = getMemForTest(64);
	p08 = getMemForTest(128);
	p09 = getMemForTest(128);
	p10 = getMemForTest(256);
	p11 = getMemForTest(256);
	p12 = getMemForTest(512);
	p13 = getMemForTest(512);
	p14 = getMemForTest(1024);
	p15 = getMemForTest(1024);
	p16 = getMemForTest(2048);
	p17 = getMemForTest(2048);
	p18 = getMemForTest(4096);
	p19 = getMemForTest(4096);

	printFreeMem();
	freeMemForTest(p20);
	freeMemForTest(p02);
	freeMemForTest(p04);
	freeMemForTest(p06);
	freeMemForTest(p08);
	freeMemForTest(p10);
	freeMemForTest(p12);
	freeMemForTest(p14);
	freeMemForTest(p16);
	freeMemForTest(p18);
	printFreeMem();
	freeMemForTest(p01);
	freeMemForTest(p03);
	freeMemForTest(p05);
	freeMemForTest(p07);
	freeMemForTest(p09);
	freeMemForTest(p11);
	freeMemForTest(p13);
	freeMemForTest(p15);
	freeMemForTest(p17);
	freeMemForTest(p19);	
	printFreeMem();
	
	p20 = getMemForTest(8);
	p01 = getMemForTest(8);
	p02 = getMemForTest(16);
	p03 = getMemForTest(16);
	p04 = getMemForTest(32);
	p05 = getMemForTest(32);
	p06 = getMemForTest(64);
	p07 = getMemForTest(64);
	p08 = getMemForTest(128);
	p09 = getMemForTest(128);
	p10 = getMemForTest(256);
	p11 = getMemForTest(256);
	p12 = getMemForTest(512);
	p13 = getMemForTest(512);
	p14 = getMemForTest(1024);
	p15 = getMemForTest(1024);
	p16 = getMemForTest(2048);
	p17 = getMemForTest(2048);
	p18 = getMemForTest(4096);
	p19 = getMemForTest(4096);

	printFreeMem();
	freeMemForTest(p20);
	freeMemForTest(p02);
	freeMemForTest(p04);
	freeMemForTest(p06);
	freeMemForTest(p08);
	freeMemForTest(p10);
	freeMemForTest(p12);
	freeMemForTest(p14);
	freeMemForTest(p16);
	freeMemForTest(p18);
	printFreeMem();
	freeMemForTest(p01);
	freeMemForTest(p03);
	freeMemForTest(p05);
	freeMemForTest(p07);
	freeMemForTest(p09);
	freeMemForTest(p11);
	freeMemForTest(p13);
	freeMemForTest(p15);
	freeMemForTest(p17);
	freeMemForTest(p19);	
	printFreeMem();
	break;
    
    case '8':
	printFreeMem();
	p01 = getMemForTest((uint)roundmb(freelist.next->length - 8));
	printFreeMem();	
	freeMemForTest(p01);
	printFreeMem();	
	break;
	
    case '9':
	printFreeMem();
	p01 = getMemForTest((uint)roundmb(freelist.next->length - 8));
	printFreeMem();	
	p02 = getMemForTest((uint)roundmb(freelist.next->length - 8));
	freeMemForTest(p01);
	freeMemForTest(p02);
	printFreeMem();	
	break;
    
    case 'a':
	printFreeMem();
	freeMemForTest(p01);
	printFreeMem();	
	break;	
 
    default:
	kprintf("\r\nNow you've done it.\r\n");
        break;
    }

    while (numproc > 2)
    {
        resched();
    }
    kprintf("\r\n===TEST END===\r\n");
    return;
}
