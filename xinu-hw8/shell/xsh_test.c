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
    //TODO: Test your O/S.
    
    int c, z;
    kprintf("0) Get 64 blocks.\r\n");
    kprintf("1) \r\n");
    kprintf("2) \r\n");
    kprintf("===TEST BEGIN===\r\n");
    // TODO: Test your operating system!
	
    c = kgetc();
    switch (c)
    {
    case '0':
	for (z = 0; z < 64; z++)
		sbGetBlock(supertab);
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
