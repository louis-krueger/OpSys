/**
 * @file memcpy.c
 * @provides memcpy
 *
 * $Id: memcpy.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Memory copy, copy a location in memory from src to dst.
 * @param dst destination location
 * @param src source location
 * @param n amount of data (in bytes) to copy
 * @return should equal n, if not something went wrong
 */
int memcpy(void *dst, void *src, int n)
{
	register int i;
	char *d = (char *)dst;
	char *s = (char *)src;

	for (i = 0; i < n; i++)
	{
		*d++ = *s++;
	}
	return (i+1);
}
