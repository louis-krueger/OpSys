/**
 * @file strlen.c
 * @provides strlen
 *
 * $Id: strlen.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Returns the number of non-NULL bytes in a string.
 * @param *s string
 * @return length of the string
 */
int strlen(const char *s)
{
	int n;

	n = 0;
	while (*s++)
		n++;

	return(n);
}
