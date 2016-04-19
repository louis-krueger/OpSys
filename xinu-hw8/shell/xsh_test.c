/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
    int c;
    int i = 0, j = 0;    
    int results[256];

    kprintf("0) Get 64 blocks, free 64 blocks.\r\n");
    kprintf("1) \r\n");
    kprintf("2) \r\n");
    kprintf("===TEST BEGIN===\r\n");
    // TODO: Test your operating system!
	
    c = kgetc();
    switch (c)
    {
    case '0':
	while(1)
	{
		c = sbGetBlock(supertab);
		kprintf("sbGetBlock() = %d\r\n",  c);
		if (SYSERR == c)
			break;
		results[i] = c;
		i++;
	}
	for (j = 0; j < i; j++)
		sbFreeBlock(supertab, results[j]);
	break;
    
    case '1':
	
	break;
	
    case '2':
    
	break;

    default:
	kprintf("\r\nNow you've done it.\r\n");
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return OK;
}
