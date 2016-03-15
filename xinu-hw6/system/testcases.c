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
#include <debug.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

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

/* BEGIN Textbook code from Ch 5 Programming Project 3, Silberschatz p. 254 */

struct boundedbuffer
{
    buffer_item buffer[BUFFER_SIZE];
    int bufferhead;
    int buffertail;

    semaphore empty;
    semaphore full;
    semaphore mutex;
};

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

int insert_item(struct boundedbuffer *bb, buffer_item item)
{
    // TODO:
    /* insert item into buffer
     * return 0 if successful, otherwise
     * return SYSERR indicating an error condition */
	if(wait((*bb).full) != OK)
	{
		return SYSERR;
	}

	if (wait((*bb).mutex) != OK)
	{
		return SYSERR;
	}
	(*bb).buffer[(*bb).buffertail] = item;			
	(*bb).buffertail = ((*bb).buffertail + 1 ) % BUFFER_SIZE;
	if(signal((*bb).empty) != OK)
	{
		return SYSERR;
	}
	if(signal((*bb).mutex) != OK)
	{
		return SYSERR;
	}
	return 0;
}

int remove_item(struct boundedbuffer *bb, buffer_item *item)
{
    // TODO:
    /* remove an object from buffer
     * placing it in item
     * return 0 if successful, otherwise
     * return SYSERR indicating an error condition */
	if(wait((*bb).empty) != OK)
	{
		return SYSERR;
	}
	if (wait((*bb).mutex) != OK)
	{
		return SYSERR;
	}
	*item = (*bb).buffer[(*bb).bufferhead];
	(*bb).bufferhead = ((*bb).bufferhead + 1) % BUFFER_SIZE;
	//kprintf("removed item [%d]\r\n", *item);
	if(signal((*bb).full) != OK)
	{
		return SYSERR;
	}
	if(signal((*bb).mutex) != OK)
	{
		return SYSERR;
	}
	return 0;
}

void producer(struct boundedbuffer *bb)
{
    //kprintf("producer created\n\r");
    buffer_item item;
    enable();
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 100);
        /* generate a random number */
        //kprintf("in producers loop\r\n");
	item = (rand() % 10000);
        if (insert_item(bb, item))
		{
		mutexAcquire();
		kprintf("report error condition\r\n");
        	mutexRelease();
		}
		else
        {
		mutexAcquire();
		kprintf("producer %d produced %d\r\n", currpid, item);
    		mutexRelease();
		}	
	}
}

void consumer(struct boundedbuffer *bb)
{	
    //kprintf("consumer created\n\r");
    buffer_item item;
    enable();
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 100);
        //kprintf("in consumers loop\r\n");
		if (remove_item(bb, &item))
    	{	
		mutexAcquire();
	   	kprintf("report error condition\r\n");
    		mutexRelease();
		} 
	   	else
    	{ 
		mutexAcquire();
	   	kprintf("consumer %d consumed %d\r\n", currpid, item);
    		mutexRelease();
		}
	}
}

/* END Textbook code from Ch 5 Programming Project 3, Silberschatz p. 254 */

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
    struct boundedbuffer bbuff;
    //#undef BUFFER_SIZE	//Don't ever does this, not even in homework assignments!
    //#define BUFFER_SIZE 10  //This was for testing the buffer logic These must be commented out 
    kprintf("q) Test simple queue\r\n");
    kprintf("0) Test 1 producer, 1 consumer, same priority\r\n");
    kprintf("1) Test 1 producer, 1 consumer, producer 10x priority\r\n");
    kprintf("2) Test 1 producer, 4 consumers, producer 10x priority\r\n");
    kprintf("3) Test 4 producers, 1 consumer, consumer 10x priority\r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
	case 'q':
        kprintf("Testing scheduling case: %c\r\n", c);
        printqueue(readylist);
	ready(create((void *)printpid, INITSTK, 5, "PRINTER-D", 1, 5),  0);
        printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 10, "PRINTER-C", 1, 5), 0);
        printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 15, "PRINTER-B", 1, 5), 0);
        printqueue(readylist);
        ready(create((void *)printpid, INITSTK, 20, "PRINTER-A", 1, 5), 0);
        printqueue(readylist);
        kprintf("end of test scheduling\r\n");
	break;
	
	case '0':
        // TODO:
        // Initialize bbuff, and create producer and consumer processes
	bbuff.bufferhead = 0;
	bbuff.buffertail = 0;
	bbuff.empty = semcreate(0);
	bbuff.full = semcreate(BUFFER_SIZE);
	bbuff.mutex = semcreate(1);				
	ready(create((void *)producer, INITSTK, 10, "producer", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer", 1, &bbuff), 0);
	break;
	
	case '1':
	bbuff.bufferhead = 0;
	bbuff.buffertail = 0;
	bbuff.empty = semcreate(0);
	bbuff.full = semcreate(BUFFER_SIZE);
	bbuff.mutex = semcreate(1);				
	ready(create((void *)producer, INITSTK, 100, "producer", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer", 1, &bbuff), 0);
	break;
	
	case '2':
	bbuff.bufferhead = 0;
	bbuff.buffertail = 0;
	bbuff.empty = semcreate(0);
	bbuff.full = semcreate(BUFFER_SIZE);
	bbuff.mutex = semcreate(1);				
	ready(create((void *)producer, INITSTK, 100, "producer", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer1", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer2", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer3", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 10, "consumer4", 1, &bbuff), 0);
	break;
	
	case '3':
	bbuff.bufferhead = 0;
	bbuff.buffertail = 0;
	bbuff.empty = semcreate(0);
	bbuff.full = semcreate(BUFFER_SIZE);
	bbuff.mutex = semcreate(1);				
	ready(create((void *)producer, INITSTK, 10, "producer1", 1, &bbuff), 0);
	ready(create((void *)producer, INITSTK, 10, "producer2", 1, &bbuff), 0);
	ready(create((void *)producer, INITSTK, 10, "producer3", 1, &bbuff), 0);
	ready(create((void *)producer, INITSTK, 10, "producer4", 1, &bbuff), 0);
	ready(create((void *)consumer, INITSTK, 100, "consumer", 1, &bbuff), 0);
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
