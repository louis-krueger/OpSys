/**
 * @file strchr.c
 * @provides strchr
 *
 * $Id: strchr.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/** 
 * Returns a pointer to the location in a string at which which a particular
 * character appears.
 * @param *s string to search
 * @param c character to locate
 * @return the pointer in the string, NULL if character not found
 */
char *strchr (const char *s, int c)
{
	for (; *s != '\0'; s++)
	{
		if (*s == (const char)c)
		{ return (char *)s; }
	}
	return 0;
}
