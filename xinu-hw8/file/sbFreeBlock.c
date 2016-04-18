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
    kprintf("sbFreeBlock(%d)\r\n", block);
    struct freeblock *freeblk, *free2;
    struct dentry *phw;
    int diskfd;

    if (NULL == psuper)
    {
	kprintf("sbFreeBlock-SYSERR1\r\n");
	return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
	kprintf("sbFreeBlock-SYSERR2\r\n");
        return SYSERR;
    }
    diskfd = phw - devtab;
    wait(psuper->sb_freelock);
    freeblk = psuper->sb_freelst;
    if (NULL == freeblk)
    //We must recreate the free block.
    {
	free2->fr_blocknum = block;
	free2->fr_count = 0;
	free2->fr_next = NULL;
	psuper->sb_freelst = free2;
	freeblk = psuper->sb_freelst;
    }
    else
    {
	while (freeblk->fr_count == FREEBLOCKMAX) 
	{
		if (freeblk->fr_next == NULL)
		{
			free2->fr_blocknum = block;
			free2->fr_count = 0;
        		free2->fr_next = NULL;
			freeblk->fr_next = free2;
			seek(diskfd, block);
			if (SYSERR == write(diskfd, free2, sizeof(struct freeblock)))
    			{
        			signal(psuper->sb_freelock);
        			return SYSERR;
    			}
    			signal(psuper->sb_freelock);
    			return OK;
		}
		else
		{
			freeblk = freeblk->fr_next;
		}
	}
	freeblk->fr_free[freeblk->fr_count] = block;
	freeblk->fr_count++;	
    }
    seek(diskfd, block);
    if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
    {
	signal(psuper->sb_freelock);
	return SYSERR;
    }
    signal(psuper->sb_freelock);
    return OK;
}
