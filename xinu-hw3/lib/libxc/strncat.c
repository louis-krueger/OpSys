/**
 * @file strncat.c
 * @provides strncat
 *
 * $Id: strncat.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Concatenate s2 on the end of s1.  S1's space must be large enough.
 * At most n characters are moved.
 * @param *s1 first string
 * @param *s2 second string
 * @param n length to concatenate
 * @return s1
 */
char *strncat(char *s1, char *s2, int n)
{
	char *os1;

	os1 = s1;
	while (*s1++)
		;
	--s1;
	while ((*s1++ = *s2++))
		if (--n < 0) {
			*--s1 = '\0';
			break;
		}
	return(os1);
}
