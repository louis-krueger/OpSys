/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

void testcases(void);

/**
 * Main thread.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
process main(void)
{
    /* Print information about the operating system  */

     
    kprintf("(main.c) This process [%d] start up has completed\r\n", currpid);

    return 0;
}
