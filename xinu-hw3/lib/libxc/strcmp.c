/**
 * @file strcmp.c
 * @provides strcmp
 *
 * $Id: strcmp.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Compare strings.
 * @param *s1 first memory location
 * @param *s2 second memory location
 * @return s1>s2: >0  s1==s2: 0  s1<s2: <0
 */
int strcmp(const char *s1, const char *s2)
{

	while (*s1 == *s2++)
		if (*s1++=='\0')
			return(0);
	return(*s1 - *--s2);
}
