/**
 * @file free.c
 * @provides free                                                     
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Free a memory block, returning it to free list.
 * @param *pmem pointer to memory block
 * @return OK on success, SYSERR on failure
 */
syscall	free(void *pmem)
{
	memblk *block = (int)pmem - 8;

	// TODO: Perform some sanity checks to see if pmem is feasible and
	//       could be from a malloc() request:
	//       1) is ptr within heap region of memory?
	//       2) is ptr + length within heap region of memory?
	//       3) does accounting block mnext field point to itself?
	//       4) is accounting block mlen field nonzero?
	//       Call freemem() to put back into free list.
	if ((block < memheap) || (block > platform.maxaddr))
		return SYSERR;
	if (block->length == 0)
		return SYSERR;
	if (block + block->length > platform.maxaddr)
		return SYSERR;
	if (block->next != &(block->next))
		return SYSERR;
	return freemem(pmem, block->length);

/*	memblk *bp, *p;
	
	bp = (memblk*)pmem - 1;
	for (p = memheap; !(bp > p && bp < p->next); p = p->next)
		if ( p >= p->next && (bp > p || bp < p->next))
			break;
	
	if (bp + bp->length == p->next)
	{
		bp->length += p->next->length;
		bp->next = p->next->next;
	}	 
	else
	{
		bp->next = p->next;
	}
	if (p + p->length == bp)
	{
		p->length += bp->length;
		p->next = bp->next;
	}
	else
	{
		p->next = bp;
	}
	memheap = p;
*/
}

