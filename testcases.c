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
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <device.h>
#include <stdio.h>
#include <uart.h>

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	int c;

	kprintf("===TEST BEGIN===");

	// TODO: Test your operating system!

	kprintf("\r\n===TEST END===\r\n");
	return;
}
