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
	if (freelist.next == &freelist)
		return (void*)SYSERR;
	/* START OF INITILIZATIONS */
	memblk* newmem;				/* new block of memory */
	memblk* freemem = freelist.next;	/* travsersal memory block */
	memblk* prevfree;			/* 2nd traversal memory block */
	nbytes = (uint)roundmb(nbytes);
	/* END OF INITILIZATIONS */
	/* START OF FREELIST TRAVERSAL */
	while(freemem->length < nbytes) 	/* traverse until we find a block large enough for nbytes */
        {
        	if (freemem->next != NULL)
                {
                	prevfree = freemem;
                        freemem = freemem->next;
                }
                else if (freemem->length != nbytes)	/* if we are at the last block in free and the new block wont fit return SYSERR*/
                	return (void *)SYSERR;
		else
			break;
                
        }
	/* END OF FREELIST TRAVERSAL */
	/* SET UP THE NEW MEMORY BLOCK */
	newmem = freemem;
	newmem->length = nbytes;
	/* END OF SETTING UP NEW MEMORY BLOCK */
	if (newmem == freelist.next)			/* if the block we find is at the beggining of freelist */
	{
		ulong temp = (ulong)(freelist.next)->next;
		if (((int)newmem + nbytes) >=(int)platform.maxaddr)
		{
			newmem->next = NULL;
			freelist.next = &freelist;
			freelist.length = NULL;
		}
        	else
		{
			newmem->next = (void *)((int)newmem + nbytes);
			freelist.next = newmem->next;
			freelist.length = (freelist.length) - (newmem->length);
        	        (freelist.next)->next = (void *)temp;
	                (freelist.next)->length = freelist.length;
		}	
	}
	else if (newmem->length < freemem->length)	/* if the block we find is in the middle and breaks up freelist */
	{
		ulong temp = (ulong)freemem->next;
		newmem->next = (void *)((int)newmem + (nbytes));
		freemem = newmem->next;
		freemem->next = (void *)temp;
		freemem->length = (freemem->length - newmem->length);
		prevfree->next = freemem;
	}
	else if (newmem->length == freemem->length)	/* if the block we find is in the middle and fits perfectly */
	{
		freemem = freemem->next;
                newmem->next = (void *)((int)newmem + (nbytes));
		prevfree->next = freemem;
	}
	return newmem;
}
