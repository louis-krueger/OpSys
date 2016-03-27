/**
 * @file getmem.c
 * @provides getmem                                                       
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap storage, returning pointer to assigned memory region.
 * @param nbytes number of bytes requested
 * @return pointer to region on success, SYSERR on failure
 */
void *getmem(uint nbytes)
{
	// TODO: Search free list for first chunk large enough to fit.
	//       Break off region of requested size; return pointer
	//       to new memory region, and add any remaining chunk
	//       back into the free list.
	memblk*  newmem;		/* new block of memory */
	memblk* freemem = freelist;	/* travsersal memory block */
	memblk* prevfree;
	int count = 0;
	while (freemem->length < nbytes)
	{
		if (count == 1)
			prevfree = freelist;
		if (freemem->next == NULL)
		{
			if (count < 1)
				count++;
			else
				prevfree = prevfree->next;
			freemem* = freemem->next;
		}
		else
			return (void *)SYSERR;
	}
	newmem = freemem;
	newmem->next = roundmb(newmem + nbytes);
	newmem->length = (ulong)trncmb((ulong)&(newmem->next) - (ulong)newmem);
	if (newmem == freelist)
	{
		freelist.next = newmem->next;
		freemem->length = (freemem->length) - (newmem->length);
	}
	else if (newmem->length < freemem->length)
	{
		*freemem = newmem->next;
		prevfree->next = freemem
	}
	else if (newmem->length == freemem->length)
	{
		*freemem = freemem->next;
		prevfree->next = *freemem;
	}
	return newmem;
}
