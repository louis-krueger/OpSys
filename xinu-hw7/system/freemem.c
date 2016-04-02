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
#define MINBYTES 8

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
	if (pmem < (void*)freelist.next)
	{
		ulong temp = (ulong)freelist.next;
		freelist.next = pmem;
		freelist.length = nbytes + MINBYTES;
		(freelist.next)->next = (void*) temp;
		(freelist.next)->length = nbytes + MINBYTES;
		return OK;
	}
	else
	{
		memblk* freemem = freelist.next;
		memblk* prevfree;
        	int count = 0;
		while ((void*)freemem < pmem)
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
			{
				freemem->next = pmem;
		                memblk* newblk = pmem;
               			newblk->length = nbytes + MINBYTES;
                		newblk->next = NULL;
				return OK;
			}
		}
		prevfree->next = pmem;
		memblk* newblk = pmem;
                newblk->length = nbytes + MINBYTES;
		newblk->next = freemem;
		return OK;
	}
	/* END OF FREELIST RECONNECTION */
	/* START OF COMPACTION */
	
	/* END OF COMPACTION */
	return SYSERR;
}
