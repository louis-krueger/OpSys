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
void philosopher(int);
void radio(int);
void thing1(int);
void thing2(int);

void bigargs(int a, int b, int c, int d, int e, int f)
{
   // enable();
    resched();
    kprintf("bigargs(%d, %d, %d, %d, %d, %d) == %d\r\n",
            a, b, c, d, e, f, a + b + c + d + e + f);
}

void printpid(int times)
{
    int i = 0;	    
    //enable();
    for (i = 0; i < times; i++)
    {
        resched();
        kprintf("This is process %d - [%s]\r\n", currpid, proctab[currpid].name);
    }
}

void thing1(int X)
{
	kprintf("I am Thing 1\r\n");
	resched();
	kprintf("\x1b[31m""I am thing red");
	resched();
	kprintf("\x1b[31m""Its a great day, how do you do?\r\n");
	resched();
	kprintf("\x1b[31m""\r\nUs things do not bite\r\n");
	resched();
	kprintf("\x1b[0m""Then, out of the box\r\n");
	resched();
	kprintf("\x1b[31m"" and Thing one!\r\n");
	resched();
}

void thing2(int X)
{
	kprintf("And I am Thing 2\r\n");
	resched();
	kprintf("\x1b[34m"" and I thing blue!\r\n");
	resched();
	kprintf("\x1b[34m""I told you thing one, I am thing two!\r\n");
	resched();
	kprintf("\x1b[34m""WE just want to have fun\r\n");
	resched();
	kprintf("\x1b[34m""Came thing two");	
	resched();
	kprintf("\x1b[0m");
	resched();
}


void philosopher(int X)
{
	while(TRUE)
	{
		enable();
		kprintf("Give me a second to think...\r\n");
	}
}

void radio(int allowedQuantums)
{
	int i = 0;	

	for(i = 0; i < allowedQuantums; i++)
	{
		resched();
	}
	kprintf("\r\nRadio killed the philosopher\r\n");
	kill(currpid - 1);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("===TEST BEGIN===\r\n");

    kprintf("0) Test priority scheduling\r\n");
    kprintf("1) Test priority scheduling, utilize aging\r\n");
    kprintf("2) Cooperative print\r\n");
    kprintf("3) Test big args\r\n");
    kprintf("p) Test preemption\r\n\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        kprintf("Testing scheduling case: %c\r\n", c);	
	ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5),  0);
        ready(create((void *)printpid, INITSTK, 10, "PRINTER-C", 1, 5), 0);
        ready(create((void *)printpid, INITSTK, 15, "PRINTER-B", 1, 5), 0);
        ready(create((void *)printpid, INITSTK, 20, "PRINTER-A", 1, 5), 0);
	kprintf("end of test scheduling\r\n");
        break;

    case '1':	//test prioritization and aging. big arg calls finish before all the printers after they have waited long enough
        kprintf("Testing scheduling case: %c\r\n", c);	
	ready(create((void *)printpid, INITSTK, 40, "PRINTER-A", 1, 20),  0);
        ready(create((void *)printpid, INITSTK, 30, "PRINTER-B", 1, 20),  0);
        ready(create((void *)printpid, INITSTK, 20, "PRINTER-C", 1, 20),  0);
        ready(create((void *)printpid, INITSTK, 10, "PRINTER-D", 1, 20),  0);
	ready(create((void *)bigargs,  INITSTK, 3, "BIGARGS-A", 6, 1, 20, 30, 40, 50, 60), 0);
        ready(create((void *)bigargs,  INITSTK, 2, "BIGARGS-B", 6, 2, 20, 30, 40, 50, 60), 0);
        ready(create((void *)bigargs,  INITSTK, 1, "BIGARGS-C", 6, 3, 20, 30, 40, 50, 60), 0);
	kprintf("end of test scheduling\r\n");
        break;
   
    case '2':	//Ahh yes, thing 1 and thing 2. Demonstrate cooperative processes
        kprintf("\r\n");	
	ready(create((void *)thing1, INITSTK, 11, "Thing1", 1, 1), 0);
	ready(create((void *)thing2, INITSTK, 10, "Thing2", 1, 2), 0);
        break;

    case '3':	//test priortization
        kprintf("Testing scheduling case: %c\r\n", c);	
	ready(create((void *)bigargs,  INITSTK, 2, "BIGARGS-A", 6, 3, 20, 30, 40, 50, 60),  0);
        ready(create((void *)bigargs,  INITSTK, 7, "BIGARGS-B", 6, 2, 20, 30, 40, 50, 60),  0);
        ready(create((void *)bigargs,  INITSTK, 20, "BIGARGS-C", 6, 1, 20, 30, 40, 50, 60), 0);
	kprintf("End of case: %c\r\n", c);
        break;
    
    case 'p':   //spawn rambling philosopher, radio gives him 100 quantums and then kills him!
        kprintf("Testing scheduling case: %c\r\n", c);	
	ready(create((void *)philosopher, INITSTK, 4, "Philosoper", 0), 0);	
	ready(create((void *)radio, INITSTK, 5, "Radio", 1, 100), 0);
	kprintf("End of case: %c\r\n", c);
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
