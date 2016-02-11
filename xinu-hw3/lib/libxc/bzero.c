/**
 * @file bzero.c
 * @provides bzero
 *
 * $Id: bzero.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Clears a block of characters to 0s
 * @param *p pointer to start of block
 * @param len length of the block
 */
void bzero(void *p, int len)
{
	int n;
	char *pch = (char *)p;

	if ((n = len) <= 0)
	{ return; }
	do
	{ *pch++ = 0; }
	while (--n);
}
