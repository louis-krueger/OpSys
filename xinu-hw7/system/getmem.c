/**
 * Written by Samuel Scheel & Louis Kruger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file getmem.c
 * @provides getmem                                                       
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>
#define MINBYTES 8		/* Number of bytes for the two accounting blocks */

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
	/* START OF INITILIZATIONS */
	memblk*  newmem;			/* new block of memory */
	memblk* freemem = freelist.next;	/* travsersal memory block */
	memblk* prevfree;
	nbytes = (uint)roundmb(nbytes);		/* number of memory addresses request size will fit into */
	int count = 0;
	/* END OF INITILIZATIONS */
	/* START OF FREELIST TRAVERSAL */
	while (freemem->length < nbytes + MINBYTES)	/* traverse until we find a block large enough for nbytes */
	{
		if (count == 1)
			prevfree = freelist.next;
		if (freemem->next != NULL)
		{
			if (count < 1)
				count++;
			else
				prevfree = prevfree->next;
			freemem = freemem->next;
		}
		else
			return (void *)SYSERR;	/* return SYSERR if we reach end of 
						   freelist with no space for new block */
	}
	/* END OF FREELIST TRAVERSAL */
	/* SET UP THE NEW MEMORY BLOCK */
	newmem = freemem;
//	newmem->next = (int)newmem + (nbytes + MINBYTES);
	newmem->length = nbytes + MINBYTES;
	/* END OF SETTING UP NEW MEMORY BLOCK */
	if (newmem == freelist.next)
	{
		ulong temp = (ulong)(freelist.next)->next;
        	newmem->next = (void *)((int)newmem + (nbytes + MINBYTES));
		freelist.next = newmem->next;
		freelist.length = (freelist.length) - (newmem->length);
		(freelist.next)->next = (void *)temp;
		(freelist.next)->length = freelist.length;
	}
	else if (newmem->length < freemem->length)
	{
		ulong temp = (ulong)freemem->next;
		newmem->next = (void *)((int)newmem + (nbytes + MINBYTES));
		freemem = newmem->next;
		freemem->next = (void *)temp;
		freemem->length = (freemem->length - newmem->length);
		prevfree->next = freemem;
	}
	else if (newmem->length == freemem->length)
	{
		freemem = freemem->next;
                newmem->next = (void *)((int)newmem + (nbytes + MINBYTES));
		prevfree->next = freemem;
	}
	return newmem;
}
