/**
 * @file sscanf.c
 * @provides sscanf, sgetch, sungetch
 *
 * $Id: sscanf.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#define EOF   (-2)

static int sgetch(int, char **);
static int sungetch(int, char **);
extern int _doscan(register char *, register int **, int (*)(), int (*)(), int, int);

/**
 * Read from a string according to a format.
 * @param *str string to read from
 * @param *fmt format string
 * @param args number of arguments in format string
 * @return result of _doscan
 */
int sscanf(char *str, char *fmt, int args)
{
	char	*s;

	s = str;
	return(_doscan(fmt, (int **)&args, sgetch, sungetch, 0, (int)&s));
}

/**
 * Get the next character from a string.
 * @param dummy unused variable
 * @param string to read next character from
 */
static int sgetch(int dummy, char **cpp)
{
	return( *(*cpp) == '\0' ? EOF : *(*cpp)++ );
}

/**
 * Pushback a character in a string.
 * @param dummy unused variable
 * @param string to pushback character to
 */
static int sungetch(int dummy, char **cpp)
{
	return(*(*cpp)--);
}
