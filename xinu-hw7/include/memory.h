/**
 * @file memory.h
 * Definitions for kernel memory allocator and maintenance.
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

/* roundmb - round address up to size of memblock  */
#define roundmb(x)  (void *)( (7 + (ulong)(x)) & ~0x07 )
/* truncmb - truncate address down to size of memblock */
#define truncmb(x)  (void *)( ((ulong)(x)) & ~0x07 )

/**
 * Structure for a block of memory.
 */
typedef struct memblock
{
    struct memblock *next;          /* pointer to next memory block       */
    uint length;                    /* size of memory block (with struct) */
} memblk;

extern memblk freelist;             /* head of free memory list           */

/* Other memory data */

extern void *_end;                  /* linker provides end of image       */
extern void *memheap;               /* bottom of heap                     */

/* Memory function prototypes */
void *getmem(uint nbytes);
syscall freemem(void *pmem, uint nbytes);

void *malloc(uint nbytes);
syscall free(void *pmem);

#endif                          /* _MEMORY_H_ */
