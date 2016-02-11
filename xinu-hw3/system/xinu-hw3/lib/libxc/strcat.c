/**
 * @file strcat.c
 * @provides strcat
 *
 * $Id: strcat.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/*
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * @param s1 first string
 * @param s2 second string
 * @return s1.
 */
char *strcat(char *s1, char *s2)
{
	char *os1;

	os1 = s1;
	while (*s1++)
	{ ; }
	--s1;
	while (((*s1++) = (*s2++)))
	{ ; }
	return os1;
}
