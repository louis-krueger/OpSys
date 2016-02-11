/**
 * @file fprintf.c
 * @provides fprintf
 *
 * $Id: fprintf.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <vararg.h>

#define	OK	1

extern void _doprnt(char *, va_list, int (*)(int, int), int);

/**
 * Print a formatted message on specified device (file)
 * @param dev device to write to
 * @param *fmt format string
 */
int fprintf(int dev, char *fmt, ...)
{
	va_list ap;
	int putc(int, char);

	va_start(ap, fmt);
    _doprnt(fmt, ap, putc, dev);
	va_end(ap);

    return OK;
}
