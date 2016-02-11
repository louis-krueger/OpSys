/**
 * @file fgets.c
 * @provides fgets
 *
 * $Id: fgets.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#define NULL    0
extern int getc(int);

/**
 * Read a newline-terminated string from device (file) dev
 * @param dev device to read from
 * @param *s buffer for result
 * @param n maximum read length
 */
char *fgets(int dev, char *s, int n)
{
	int c = 0;
	char *cs;
	cs = s;

	/* Read characters until maximum read length, */
	/*  end of line, or end of file               */
    while ((--n>0) && ((c = getc(dev))>=0)) 
	{
		*cs++ = c;
		if (('\n' == c) || ('\r' == c))
		{ break; }
	}
	
	/* Check for EOF or empty string */
	if ((c<0) && (cs==s))
	{ return NULL; }

	/* Terminate string and return */
	*cs++ = '\0';
	return s;
}
