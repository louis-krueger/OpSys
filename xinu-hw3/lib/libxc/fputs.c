/**
 * @file fputs.c
 * @provides fputs
 *
 * $Id: fputs.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

extern int putc(int, char);

/**
 * Write a null-terminated string to a device (file)
 * @param dev device to write to
 * @param *s string to write
 * @return result of last putc
 */
int fputs(int dev, char *s)
{
	int r=0, c;

	while ((c = (*s++)))
    { r = putc(dev, c); }
	return r;
}
