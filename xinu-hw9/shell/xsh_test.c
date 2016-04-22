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

    kprintf("t) Dev-test.\r\n");
    kprintf("-) Get one block.\r\n");
    kprintf("!) Opposite of Dev-test.\r\n");
    kprintf("+) Free one block.\r\n");
    kprintf("0) Get every block, free every block in the same order.\r\n");
    kprintf("1) Get every block.\r\n");
    kprintf("2) Free every block in sequential order (Extends Test 1).\r\n");
    kprintf("3) Get two blocks, free two blocks (sequential free order).\r\n");
    kprintf("4) Get two blocks, free two blocks (opposite free order).\r\n");
    kprintf("===TEST BEGIN===\r\n");
    // TODO: Test your operating system!
	
    c = kgetc();
    switch (c)
    {
    case 't':
	for (i = 0; i < 61; i++)
	{
		c = sbGetBlock(supertab);
                kprintf("sbGetBlock() = %d\r\n",  c);
                if (SYSERR == c)
                        break;
                results[i] = c;
	}
	break;
    case '-':
	c = sbGetBlock(supertab);
        kprintf("sbGetBlock() = %d\r\n",  c);
        break;
    case '!':
	for (i = 3; i <= 63; i++)
        {
                sbFreeBlock(supertab, i);
                kprintf("sbFreeBlock(%d)\r\n",  i);
        }
        break;
    case '+':
        sbFreeBlock(supertab, 64);
        kprintf("sbGetBlock(%d)\r\n",  64);
        break;
    case 'n':
	sbFreeBlock(supertab, 2);
	sbFreeBlock(supertab, 251);


	break;
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
	{
		kprintf("sbFreeBlock(%d)\r\n", results[j]);
		sbFreeBlock(supertab, results[j]);
	}
	break;
    
    case '1':
	while(1)
        {
                c = sbGetBlock(supertab);
                kprintf("sbGetBlock() = %d\r\n",  c);
                if (SYSERR == c)
                        break;
        }	
	break;
	
    case '2':
	for (j = 2; j < 256; j++)
        {
                kprintf("sbFreeBlock(%d)\r\n", j);
                sbFreeBlock(supertab, j);
        }
	break;
    case '3':
	i = sbGetBlock(supertab);
	j = sbGetBlock(supertab);
	sbFreeBlock(supertab, i);
	sbFreeBlock(supertab, j);
	break;
    case '4':
	i = sbGetBlock(supertab);
	j = sbGetBlock(supertab);
	sbFreeBlock(supertab, j);
	sbFreeBlock(supertab, i);
	break;
    default:
	kprintf("\r\nNow you've done it.\r\n");
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return OK;
}
