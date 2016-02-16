/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 * Louis Krueger
 * and
 * Sam Scheel
 * TA-BOT:MAILTO louis.krueger@marquette.edu samuel.scheel@marquette.edu
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

devcall putc(int dev, char c) { return 0; }


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	int c;
	int i;
	char teststring[] = "Hello Wo

	kprintf("===TEST BEGIN===\r\n");
    	
	kprintf("%d", kcheckc());
    	
	for (i = 32;i < 128; i++)
		kprintf("%c", i);
    	kprintf("\r\n==========\r\n");
    	
	

	/*    switch (c)
    	{
		case 
			break;
	    // TODO: Test your operating system!
		default:
	kprintf("Hello Xinu World!\r\n");
    	}
	*/
    	kprintf("\r\n===TEST END===\r\n");
    	return;
}
