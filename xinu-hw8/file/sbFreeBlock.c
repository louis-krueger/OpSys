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
    struct freeblock *free2;
    struct dentry *phw;
    struct freeblock *swizzle;
    struct dirblock *swizzle2;
    int diskfd;
    // A little error checking...
    if (NULL == psuper)
    {
	return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    // Establish disk location and mutual exclusion
    diskfd = phw - devtab;
    wait(psuper->sb_freelock);
    freeblk = psuper->sb_freelst;
    if (NULL == freeblk)
    // We must recreate the free block.
    {
	// Setting up new collector node
	free2 = malloc(sizeof(struct freeblock));
	free2->fr_blocknum = block;
	free2->fr_count = 0;
	free2->fr_next = NULL;
	psuper->sb_freelst = free2;
	freeblk = psuper->sb_freelst;
	// Swizzle the supernode's information
	swizzle = psuper->sb_freelst;
        psuper->sb_freelst = (struct freeblock *)swizzle->fr_blocknum;
	swizzle2 = psuper->sb_dirlst;
        psuper->sb_dirlst = (struct dirblock *)swizzle2->db_blocknum;
	// Commit changes of supernode to disk
	seek(diskfd, psuper->sb_blocknum);
        if (SYSERR == write(diskfd, psuper, sizeof(struct superblock)))
        {
        	signal(psuper->sb_freelock);
        	return SYSERR;
    	}
	psuper->sb_freelst = swizzle;
	psuper->sb_dirlst = swizzle2;
    	signal(psuper->sb_freelock);
    	return OK;	
    }
    else
    // The first collector node is already established
    {
	while (freeblk->fr_next != NULL)
	// Traverse collector nodes until we find one we can add to
	{
		freeblk = freeblk->fr_next;
	}
	if (freeblk->fr_count < FREEBLOCKMAX)
	{
		freeblk->fr_free[freeblk->fr_count] = block;
                freeblk->fr_count++;
                seek(diskfd, freeblk->fr_blocknum);
                if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
                {
                        signal(psuper->sb_freelock);
                        return SYSERR;
                }
                signal(psuper->sb_freelock);
                return OK;
	}
	else
	// If the end collector node is full and we need to make a new one
	{
		// Setting up new collector node
		free2 = malloc(sizeof(struct freeblock));
                free2->fr_blocknum = block;
                free2->fr_count = 0;
                free2->fr_next = NULL;
                freeblk->fr_next = free2;
                // Swizzle the previous collector node's information
                swizzle = freeblk->fr_next;
                freeblk->fr_next = (struct freeblock *)swizzle->fr_blocknum;
                // Commit changes of collector node to disk
                seek(diskfd, freeblk->fr_blocknum);
                if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
                {
                	signal(psuper->sb_freelock);
                        return SYSERR;
                }
                freeblk->fr_next = swizzle;
                signal(psuper->sb_freelock);
                return OK;
	}
    }
}
