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
void *getMemForTest(uint nbytes);
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
   kprintf("\tcurrent is the tail of heap, calcuatled end [0x07ffffff] - 0x%08X\r\n", (uint)freemem + (uint)freemem->length - 1);
   kprintf("***End Print***\r\n");
}
void *getMemForTest(uint nbytes)
{
	void* T = getmem(nbytes);
	kprintf("Requested %d bytes.\r\n", nbytes);
	return T;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
 //   void* pS1;
  //  void* pS2;    

    kprintf("s) Very simple test case get two chunks free first\r\n");
    kprintf("1) Get-Free Test \r\n");
    kprintf("2) Get-Free-Get Test1 \r\n");
    kprintf("3) Get-Free-Get Test2 \r\n");
    kprintf("4) Get-Full Get Test \r\n");
    kprintf("5) 20 request of differing sizes, 10 freed expected addresses and size, see comment in git\r\n");
    kprintf("===TEST BEGIN===\r\n");
    // TODO: Test your operating system!
	
    c = kgetc();
    switch (c)
    {
    case 's':
	printFreeMem();

//	pS1 = malloc(16);
        printFreeMem();
//	pS2 = malloc(0);
//	printFreeMem();

//	freemem(pS2, 0);
//	kprintf("Freed 8 bytes.\r\n");
//        printFreeMem();
	//freemem(pS1, 0);
	break;
    
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
	freemem(p2, 16);
	freemem(p4, 16);
	freemem(p6, 16);
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
       	freemem(p11, 0);
	freemem(p13, 16);
	freemem(p00, 16);
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
        freemem(p21, 0);
	freemem(p23, 16);
	freemem(p20, 8);
	break;

	
    case '4':
        printFreeMem();

        void* p31 = getmem(0);
        kprintf("Requested 0 bytes.\r\n");

	void* p32 = getmem(roundmb(0x07FFFFF7));
	kprintf("Requested 0x%08X  bytes\r\n", roundmb(0x07FFFFF7));
	printFreeMem();	
	freemem(p32, roundmb(0x07FFFFF7));
	printFreeMem();
	freemem(p31, 0);
	break;

    case '5':
        printFreeMem();
	
	void* p400 = getMemForTest(8);
	void* p401 = getMemForTest(8);
	void* p402 = getMemForTest(16);
	void* p403 = getMemForTest(16);
	void* p404 = getMemForTest(32);
	void* p405 = getMemForTest(32);
	void* p406 = getMemForTest(64);
	void* p407 = getMemForTest(64);
	void* p408 = getMemForTest(128);
	void* p409 = getMemForTest(128);
	void* p410 = getMemForTest(256);
	void* p411 = getMemForTest(256);
	void* p412 = getMemForTest(512);
	void* p413 = getMemForTest(512);
	void* p414 = getMemForTest(1024);
	void* p415 = getMemForTest(1024);
	void* p416 = getMemForTest(2048);
	void* p417 = getMemForTest(2048);
	void* p418 = getMemForTest(4096);
	void* p419 = getMemForTest(4096);

	printFreeMem();
	freemem(p400, 8);
	freemem(p402, 16);
	freemem(p404, 32);
	freemem(p406, 64);
	freemem(p408, 128);
	freemem(p410, 256);
	freemem(p412, 512);
	freemem(p414, 1024);
	freemem(p416, 2048);
	freemem(p418, 4096);
	printFreeMem();
	freemem(p401, 8);
	freemem(p403, 16);
	freemem(p405, 32);
	freemem(p407, 64);
	freemem(p409, 128);
	freemem(p411, 256);
	freemem(p413, 512);
	freemem(p415, 1024);
	freemem(p417, 2048);
	freemem(p419, 4096);	
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
