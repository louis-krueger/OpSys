/**
 * Written by Samuel Scheel & Louis Kruger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file freemem.c
 * @provides freemem                                                     
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Free a memory block, returning it to free list.
 * @param pmem pointer to memory block
 * @param nbytes size of memory block
 * @return OK on success, SYSERR on failure
 */
syscall	freemem(void *pmem, uint nbytes)
{
	// TODO: Insert back into free list, and compact with adjacent blocks.
	/* START OF FREELIST RECONNECTION */
	nbytes = (uint)roundmb(nbytes);
	memblk* freemem = freelist.next;
        memblk* prevfree;
	if (pmem < (void*)freelist.next)
	{
		ulong temp = (ulong)freelist.next;
		freelist.next = pmem;
		freelist.length = nbytes;
		(freelist.next)->next = (void*) temp;
		(freelist.next)->length = nbytes;
		prevfree = freelist.next;
		freemem = prevfree->next;
	}
	else if (freelist.next == &freelist)
	{
                memblk* newblk = pmem;
                newblk->length = nbytes;
                newblk->next = NULL;
                freelist.next = newblk;
	}
	else
	{
		while((void*)freemem < pmem)
		{
			prevfree = freemem;
                        freemem = freemem->next;
			if (freemem->next == NULL)
			{
				break;
			}
		}	
		prevfree->next = pmem;
		memblk* newblk = pmem;
                newblk->length = nbytes;
		newblk->next = freemem;
		freemem = prevfree->next;
	}
	/* END OF FREELIST RECONNECTION */
	/* START OF COMPACTION */
	if (((int)freemem + freemem->length) == ((int)freemem->next))
        {
		freemem->length = freemem->length + (freemem->next)->length;
                freemem->next = (freemem->next)->next;
        }
	if (((int)prevfree + prevfree->length) == ((int)freemem))
	{
		prevfree->next = freemem->next;
        	prevfree->length = prevfree->length + freemem->length;
	}
	/* END OF COMPACTION */
	return OK;
}
