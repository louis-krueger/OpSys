/**
 * @file sprintf.c
 * @provides sprintf
 *
 * $Id: sprintf.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <vararg.h>

static char sprntf(char **, char);
extern void _doprnt(char *, va_list, int (*)(int, int), int);

/**
 * Format arguments and place output in a string
 * @param *str output string
 * @param *fmt format string
 */
int sprintf(char *str, char *fmt, ...)
{
	va_list ap;
    char    *s;
	
    s = str;
	va_start(ap, fmt);
    _doprnt(fmt, ap, sprntf, (int)&s);
	va_end(ap);
    *s++ = '\0';

	return ((int)str);
}

/**
 * Routine called by _doprnt to handle each character
 */
static char sprntf(char **cpp, char c)
{
    return (*(*cpp)++ = c);
}


