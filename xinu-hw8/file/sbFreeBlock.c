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
    wait(supertab->sb_freelock);
    freeblk = supertab->sb_freelst;
    while (freeblk->fr_count != FREEBLOCKMAX) 
    {
	if (freeblk->fr_next == NULL)
		break;
	freeblk = freeblk->fr_next;
    }
    seek(diskfd, block);
    write(diskfd, psuper, sizeof(struct freeblock));
    signal(supertab->sb_freelock);
    return SYSERR;
}
