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
	if (pmem < (void*)freelist.next)		/* if our pointer is behind freelist's start */
	{
		ulong temp = (ulong)freelist.next;
		freelist.next = pmem;
		freelist.length = nbytes;
		(freelist.next)->next = (void*) temp;
		(freelist.next)->length = nbytes;
		prevfree = freelist.next;
		freemem = prevfree->next;
	}
	else if (freelist.next == &freelist)		/* error check if freelist is full */
	{
                memblk* newblk = pmem;
                newblk->length = nbytes;
                newblk->next = NULL;
                freelist.next = newblk;
	}
	else
	{
		while((void*)freemem < pmem)		/* traverse until we find the block ahead and behind */
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
	if (((int)freemem + freemem->length) == ((int)freemem->next))			/* if freed block can be compacted forward */
        {
		freemem->length = freemem->length + (freemem->next)->length;
                freemem->next = (freemem->next)->next;
        }
	if (((int)prevfree + prevfree->length) == ((int)freemem))			/* if freed block can be compacted backward */
	{
		prevfree->next = freemem->next;
        	prevfree->length = prevfree->length + freemem->length;
	}
	if (((int)freelist.next + freelist.length) == (int)freemem)			/* if freelist needs to be updated with compaction */
        {
                (freelist.next)->next = freemem->next;
                freelist.length = freelist.length + freemem->length;
        }
	/* END OF COMPACTION */
	return OK;
}
