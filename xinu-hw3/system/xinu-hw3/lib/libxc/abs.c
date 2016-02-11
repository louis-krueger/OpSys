/**
 * @file abs.c
 * @provides abs
 *
 * $Id: abs.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Calculates the absolute value of a number.
 * @param arg number to get absolute value of
 * @return absolute value of arg
 */
int abs(int arg)
{
	if (arg < 0)
		arg = -arg;
	return(arg);
}
