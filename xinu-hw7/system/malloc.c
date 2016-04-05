/**
 * @file malloc.c
 * @provides malloc                                                       
 *
 * COSC 3250 / COEN 4820 malloc project.
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>
#define NALLOC 8              /* Number of bytes for the two accounting blocks */

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
	nbytes = (uint)roundmb(nbytes) + NALLOC;
	memblk* newmem = (void *)(int)getmem(nbytes);
	if ((int)newmem == SYSERR)
		return (void *)SYSERR;
	newmem->next = (void *)&(newmem->next);		/* set up accounting data */
	newmem->length = nbytes;
	return (void *)((int)newmem + 8);		/* move pointer above accounting information */
}
