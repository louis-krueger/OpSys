/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>


void bigargs(int a, int b, int c, int d, int e, int f)
{
    resched();
    kprintf("bigargs(%d, %d, %d, %d, %d, %d) == %d\r\n",
            a, b, c, d, e, f, a + b + c + d + e + f);
}

void printpid(int times)
{
    int i = 0;	    
    //enable();
    //resched();
    for (i = 0; i < times; i++)
    {
        resched();
        kprintf("This is process %d\r\n", currpid);
    }
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("===TEST BEGIN===\r\n");

    kprintf("0) Test priority scheduling\r\n");


    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        kprintf("testing scheduling\r\n");
	ready(create((void *)printpid, INITSTK, 2, "PRINTER-A", 1, 5), 1);
        ready(create((void *)printpid, INITSTK, 3, "PRINTER-B", 1, 5), 1);
        ready(create((void *)printpid, INITSTK, 4, "PRINTER-C", 1, 5),1);
        ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5), 1);
	ready(create((void *)bigargs,  INITSTK, 6, "BIGARGS-A", 6, 6, 20, 30, 40, 50, 60), 1);
        ready(create((void *)bigargs,  INITSTK, 6, "BIGARGS-B", 6, 6, 20, 30, 40, 50, 60), 1);
        ready(create((void *)bigargs,  INITSTK, 6, "BIGARGS-C", 6, 6, 20, 30, 40, 50, 60), 1);
	kprintf("end of test scheduling\r\n");
        break;

    default:
        break;
    }

    while (numproc > 2)
    {
        resched();
    }
    kprintf("\r\n===TEST END===\r\n");
    return;
}
