/**
 * @file malloc.c
 * @provides malloc                                                       
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>
static memblk base;
static memblk* freep = NULL;

/**
 * Allocate heap storage plus accounting block, returning pointer to
 * assigned memory region.
 * @param nbytes number of bytes requested
 * @return pointer to region on success, SYSERR on failure
 */
void *malloc(uint nbytes)
{
	// TODO: Pad request size with room for accounting info.
	//       Call getmem() to allocate region.
	//       Store accounting block at head of region, including size
	//         of request.  Return pointer to space above accounting
	//         block.
	
	memblk *p, *prevp;
	unsigned nunits;
	
	nunits = (nbytes + sizeof(memblk)-1)/sizeof(memblk) + 1;
	if ((prevp = freep) == NULL)
	{
		base->next = freep = prevp = &base;
		base->length = 0;
	}
	for (p = prev->next; ; prevp = p, p = p->next)
	{
		if (p->length >= munits)
		{
			if (p->length == nunits)
				prev->next = p->next;
			else {
				p->length -= nunits;
				p += p.length;
				p->length = nunits;
			}
			freep = prevp;
			return (void*)(p+1);
		}
		if (p == freep)
			if ((p = getmem(nunits)) == NULL)
				return NULL;
	}

}
