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
   kprintf("\tcurrent is the tail of heap, calcuatled end [0x07ffffff] - 0x%08X\r\n", (uint)freemem + (uint)freemem->length);
   kprintf("***End Print***\r\n");
}
void *getMemForTest(uint nbytes)
{
	void* T = malloc(nbytes);
	kprintf("Requested %d bytes.\r\n", nbytes);
	return T;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    void* pS1;
    void* pS2;
    void* pS3;    

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
	pS1 = malloc(16);
        printFreeMem();
	pS2 = malloc(32);
	printFreeMem();
	pS3 = malloc(64);
	printFreeMem();
	free(pS1);
	printFreeMem();
	free(pS2);
        //printFreeMem();
	free(pS3);
        //printFreeMem();
	break;
    
    case '1':
	printFreeMem();

	void* p1 = malloc(0);
	kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

	void* p2 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p3 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p4 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p5 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
	
	void* p6 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        printFreeMem();

	free(p1);
	kprintf("Freed 0 bytes.\r\n");
        free(p3);
	kprintf("Freed 16 bytes.\r\n");
	free(p5);
        kprintf("Freed 16 bytes.\r\n");

        printFreeMem();
	free(p2);
	free(p4);
	free(p6);
	break;
	
    case '2':
        printFreeMem();

        void* p11 = malloc(0);
        kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

        void* p12 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
	
        printFreeMem();

        void* p13 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        void* p14 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
        
	free(p12);
        kprintf("Freed p12.\r\n");
	
	free(p14);
        kprintf("Freed p14.\r\n");
        
        printFreeMem();
        void* p00 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();
       	free(p11);
	free(p13);
	free(p00);
	break;

    case '3':
        printFreeMem();

        void* p21 = malloc(0);
        kprintf("Requested 0 bytes.\r\n");

        printFreeMem();

        void* p22 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");

        printFreeMem();

        void* p23 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        void* p24 = malloc(16);
        kprintf("Requested 16 bytes.\r\n");
        printFreeMem();

        free(p22);
        kprintf("Freed p22.\r\n");

        free(p24);
        kprintf("Freed p24.\r\n");

        printFreeMem();
        void* p20 = malloc(8);
        kprintf("Requested 8 bytes.\r\n");
        printFreeMem();
        free(p21);
	free(p23);
	free(p20);
	break;

	
    case '4':
        printFreeMem();

        void* p31 = malloc(8);
        kprintf("Requested 8 bytes.\r\n");
	printFreeMem();

	void* p32 = malloc((uint)roundmb(0x07FFFFFF - (int)p31 - 24));
	kprintf("Requested 0x%08X  bytes\r\n", (uint)roundmb(0x07FFFFFF - (int)p31 - 24));
	printFreeMem();	

	free(p32);
	printFreeMem();

	free(p31);
	printFreeMem();
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
	free(p400);
	free(p402);
	free(p404);
	free(p406);
	free(p408);
	free(p410);
	free(p412);
	free(p414);
	free(p416);
	free(p418);
	printFreeMem();
	free(p401);
	free(p403);
	free(p405);
	free(p407);
	free(p409);
	free(p411);
	free(p413);
	free(p415);
	free(p417);
	free(p419);	
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
