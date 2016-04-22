/**
 * @file memory.h
 * @provides roundew, truncew , roundmb, truncmb
 *
 * COSC 3250 / COEN 4820 Project 8.
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <kernel.h>

typedef struct memblock
{
    struct memblock *next;      /**< pointer to next memory block        */
    ulong length;               /**< size of memory block (incl. struct) */
} memblk;

extern memblk freelist;     /**< head of free memory list            */

extern void *minheap;       /**< bottom of heap                      */
extern void *end;           /**< linker provides end of image        */

/* Memory function prototypes */
extern void *getmem(ulong nbytes);
extern syscall freemem(void *pmem, ulong nbytes);

extern void *malloc(ulong nbytes);
extern syscall free(void *pmem);

/**
 * roundew - round address to next even word (add 3 to round up)
 * truncew - trunctate address to next even word
 * Binary & with ~0x03 to knock off the last 2 bits
 * @param x address to round/truncate
 */
#define	roundew(x)	(void *)( (3 + (ulong)(x)) & ~0x03 )
#define	truncew(x)	(void *)( ((ulong)(x)) & ~0x03 )

/**
 * roundmb - round address up to size of memblock
 *           (add sizeof(memblock)-1 to round up)
 * truncmb - truncate address up to size of memblock
 * Binary & with ~0x07 to knock off the last 3 bits
 * @param x address to round/truncate
 */
#define	roundmb(x)	(void *)( (7 + (ulong)(x)) & ~0x07 )
#define	truncmb(x)	(void *)( ((ulong)(x)) & ~0x07 )

#endif                          /* _MEMORY_H_ */
