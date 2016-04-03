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
#define NALLOC 8

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
	kprintf("debug [freemem] pmem - 0x%08X\r\nfreelist.next - 0x%08X\r\n", pmem, (void*)freelist.next);

	if (pmem < (void*)freelist.next)
	{
		kprintf("debug [freemem < next] pmem - 0x%08X\r\nfreelist.next - 0x%08X\r\n", pmem, (void*)freelist.next);
		ulong temp = (ulong)freelist.next;
		freelist.next = pmem;
		freelist.length = nbytes + NALLOC;
		(freelist.next)->next = (void*) temp;
		(freelist.next)->length = nbytes + NALLOC;
		return OK;
	}
	else
	{
		kprintf("debug [freemem else] pmem - 0x%08X\r\nfreelist.next - 0x%08X\r\n", pmem, (void*)freelist.next);
		memblk* freemem = freelist.next;
		memblk* prevfree;
	
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
                newblk->length = nbytes + NALLOC;
		newblk->next = freemem;
		return OK;
	}
	/* END OF FREELIST RECONNECTION */
	/* START OF COMPACTION */
	/* END OF COMPACTION */
	return SYSERR;
}
