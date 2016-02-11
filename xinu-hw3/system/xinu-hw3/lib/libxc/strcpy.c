/**
 * @file strcpy.c
 * @provides strcpy
 *
 * $Id: strcpy.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Copy string s2 to s1.  s1 must be large enough.
 * @param s1 first string
 * @param s2 second string
 * @return s1
 */
char *strcpy(char *s1, const char *s2)
{
	char *os1;

	os1 = s1;
	while (((*s1++) = (*s2++)))
		;
	return(os1);
}
