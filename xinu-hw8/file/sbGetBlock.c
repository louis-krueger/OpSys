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
        return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
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
        result = freeblk->fr_free[freeblk->fr_count];
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
            return SYSERR;
        }
        freeblk->fr_next = free2;
        if (!result)
            result = SYSERR;
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
                return SYSERR;
            }
            psuper->sb_dirlst = swizzle;
            result = freeblk->fr_blocknum;
            free(freeblk);
            signal(psuper->sb_freelock);
            return result;
        }
        // Copy over contents of next free block list segment,
        //  and then give out the block containing that segment.
        free2 = freeblk->fr_next;
        result = free2->fr_blocknum;
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
            return SYSERR;
        }
        freeblk->fr_next = free2;

    }

    signal(psuper->sb_freelock);
    return result;
}
