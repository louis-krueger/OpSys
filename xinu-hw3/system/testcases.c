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
	//int c;
	int i;
	char teststring[] = "Hello World\0";
	char *testptr = &teststring[0];

	kprintf("===TEST BEGIN===\r\n");
    	
	kprintf("kcheckc:%d\r\n", kcheckc());
    	
	for (i = 32;i < 128; i++)
		kprintf("%c", i);
    	kprintf("\r\n==========\r\n");
    	
	do{
		kprintf("(0x%08X): %c %3d 0x%x [ptr-address:0x%08X]\r\n", testptr, *testptr, *testptr, *testptr, &testptr);
	}while(*testptr++ != 0);	

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
