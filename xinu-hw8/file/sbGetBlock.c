/* sbGetBlock.c - sbGetBlock */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <device.h>
#include <disk.h>
#include <file.h>
#include <stdio.h>
#include <memory.h>

/*------------------------------------------------------------------------
 * sbGetBlock - Get a free disk block from the superblock's free list.
 *------------------------------------------------------------------------
 */
devcall sbGetBlock(struct superblock *psuper)
{
    struct freeblock *freeblk, *free2;
    struct dirblock *swizzle;
    struct dentry *phw;
    int result, i;
    int diskfd;

    if (NULL == psuper)
    {
	kprintf("sbGetBlock-SYSERR1\r\n");
        return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
	kprintf("sbGetBlock-SYSERR2\r\n");
        return SYSERR;
    }
    diskfd = phw - devtab;
    freeblk = psuper->sb_freelst;
    if (NULL == freeblk)
    {
        printf("sbGetFree() ERROR: Superblock free list is empty!\n");
        return SYSERR;
    }

    // Updating the free list must be mutually exclusive.
    wait(psuper->sb_freelock);
    // See if this first free block has some entries left.
    if (freeblk->fr_count > 0)
    {
        freeblk->fr_count--;
	kprintf("sbGetBlock-1result pre: %d\r\n", result);
        result = freeblk->fr_free[freeblk->fr_count];
	kprintf("sbGetBlock-1result post: %d\r\n", result);
        freeblk->fr_free[freeblk->fr_count] = 0;

        // Update this free block record on disk.
        free2 = freeblk->fr_next;
        if (NULL == freeblk->fr_next)
        {
            freeblk->fr_next = 0;
        }
        else
        {
            freeblk->fr_next =
                (struct freeblock *)freeblk->fr_next->fr_blocknum;
        }
        seek(diskfd, freeblk->fr_blocknum);
        if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
        {
	    kprintf("sbGetBlock-SYSERR3\r\n");
            return SYSERR;
        }
        freeblk->fr_next = free2;
        if (!result)
	{
	    kprintf("sbGetBlock-!result error: %d\r\n", result);
            result = SYSERR;
	}
    }
    else
        // The first free block list segment is empty.
    {
        // If the next list segment is null, then we're all out.
        if (NULL == freeblk->fr_next)
        {
            psuper->sb_freelst = NULL;
            swizzle = psuper->sb_dirlst;
            psuper->sb_dirlst = (struct dirblock *)swizzle->db_blocknum;
            seek(diskfd, psuper->sb_blocknum);
            if (SYSERR ==
                write(diskfd, psuper, sizeof(struct superblock)))
            {
		kprintf("sbGetBlock-SYSERR4\r\n");
                return SYSERR;
            }
            psuper->sb_dirlst = swizzle;
	    kprintf("sbGetBlock-2result pre: %d\r\n", result);
            result = freeblk->fr_blocknum;
	    kprintf("sbGetBlock-2result post: %d\r\n", result);
            free(freeblk);
            signal(psuper->sb_freelock);
	    kprintf("sbGetBlock-result1\r\n");
            return result;
        }
        // Copy over contents of next free block list segment,
        //  and then give out the block containing that segment.
        free2 = freeblk->fr_next;
	kprintf("sbGetBlock-3result pre: %d\r\n", result);
        result = free2->fr_blocknum;
	kprintf("sbGetBlock-3result post: %d\r\n", result);
        for (i = free2->fr_count - 1; i >= 0; i--)
        {
            freeblk->fr_free[i] = free2->fr_free[i];
        }
        freeblk->fr_count = free2->fr_count;
        freeblk->fr_next = free2->fr_next;
        free(free2);

        // Update this free block record on disk.
        free2 = freeblk->fr_next;
        if (NULL == freeblk->fr_next)
        {
            freeblk->fr_next = 0;
        }
        else
        {
            freeblk->fr_next =
                (struct freeblock *)freeblk->fr_next->fr_blocknum;
        }
        seek(diskfd, freeblk->fr_blocknum);
        if (SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
        {
	    kprintf("sbGetBlock-SYSERR5\r\n");
            return SYSERR;
        }
        freeblk->fr_next = free2;

    }

    signal(psuper->sb_freelock);
    kprintf("sbGetBlock-result2\r\n");
    return result;
}
