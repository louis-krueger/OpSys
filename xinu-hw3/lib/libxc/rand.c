/**
 * @file rand.c
 * @provides srand, rand
 *
 * $Id: rand.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

static unsigned long randx = 1;

/**
 * Sets the random seed.
 * @param x random seed
 */
void srand(unsigned long x)
{
	randx = x;
}

/**
 * Generates a random long.
 * @return random long
 */
unsigned long rand(void)
{
	return (((randx = randx*1103515245 + 12345)>>16) & 077777);
}
