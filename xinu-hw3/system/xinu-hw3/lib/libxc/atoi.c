/**
 * @file atoi.c
 * @provides atoi
 *
 * $Id: atoi.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * Converts an ascii value to an integer.
 * @param *p pointer to ascii string
 * @return integer
 */
int atoi(char *p)
{
	int n = 0, f = 0;

	for(;;p++) 
	{
		switch(*p) 
		{
			case ' ':
			case '\t':
				continue;
			case '-':
				f++;
			case '+':
				p++;
		}
		break;
	}
	while(*p >= '0' && *p <= '9')
	{ n = n*10 + *p++ - '0'; }
	return(f? -n: n);
}
