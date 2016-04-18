/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/* Samuel Scheel                                                   */
/* and                                                             */
/* Louis Krueger                                                   */
/* TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu */

#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
    struct freeblock *freeblk;
    struct dentry *phw;
    int diskfd;
    int result;

    if (NULL == psuper)
    {
        return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    diskfd = phw - devtab;
    wait(psuper->sb_freelock);
    freeblk = psuper->sb_freelst;
    while (freeblk->fr_count == FREEBLOCKMAX) 
    {
	if (freeblk->fr_next == NULL)
	{
		signal(psuper->sb_freelock);
		return SYSERR;
	}
	freeblk = freeblk->fr_next;
    }
    freeblk->fr_free[freeblk->fr_count] = block;
    freeblk->fr_count++;
    seek(diskfd, block);
    if (SYSERR == write(diskfd, psuper, sizeof(struct freeblock)))
    {
	signal(psuper->sb_freelock);
	return SYSERR;
    }
    signal(psuper->sb_freelock);
    return OK;
}
