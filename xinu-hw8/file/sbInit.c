/* sbInit.c - sbInit */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <device.h>
#include <disk.h>
#include <file.h>
#include <stdio.h>
#include <memory.h>

/*------------------------------------------------------------------------
 * sbInit - Initialize a fresh filesystem superblock
 *------------------------------------------------------------------------
 */
devcall sbInit(struct superblock *psuper, int diskfd)
{
    struct freeblock *freeblk = NULL;
    struct dirblock *dirblk = NULL;
    int i, j, curblock;

    printf("Initializing filesystem superblock.\n");
    // Initialize disk.
    psuper->sb_blocknum = SUPERBLOCKNUM;
    psuper->sb_blocktotal = DISKBLOCKTOTAL;
    psuper->sb_dirlst = (struct dirblock *)(SUPERBLOCKNUM + 1);
    psuper->sb_freelst = (struct freeblock *)(SUPERBLOCKNUM + 2);
    // Write superblock to disk block 0.
    write(diskfd, psuper, sizeof(struct superblock));

    // Initialize list of free disk blocks.
    curblock = (int)psuper->sb_freelst;
    i = curblock + 1;
    j = 0;
    freeblk = psuper->sb_freelst = malloc(sizeof(struct freeblock));
    if (NULL == freeblk)
    {
        return SYSERR;
    }
    do
    {
        freeblk->fr_free[j] = i;
        i++;
        j++;
        if (j == FREEBLOCKMAX)
        {
            freeblk->fr_blocknum = curblock;
            freeblk->fr_count = j;
            freeblk->fr_next = (struct freeblock *)i;
            seek(diskfd, curblock);
            write(diskfd, freeblk, sizeof(struct freeblock));
            curblock = i;
            i = curblock + 1;
            j = 0;
            freeblk = freeblk->fr_next = malloc(sizeof(struct freeblock));
            if (NULL == freeblk)
            {
                return SYSERR;
            }
        }
    }
    while (i < DISKBLOCKTOTAL);
    // Remainder of the free list.
    if (DISKBLOCKTOTAL != curblock)
    {
        freeblk->fr_blocknum = curblock;
        freeblk->fr_count = j;
        freeblk->fr_next = NULL;
        seek(diskfd, curblock);
        write(diskfd, freeblk, sizeof(struct freeblock));
    }


    // Initialize directory structure
    curblock = (int)psuper->sb_dirlst;
    psuper->sb_dirlst = dirblk = malloc(sizeof(struct dirblock));
    if (NULL == dirblk)
    {
        return SYSERR;
    }
    dirblk->db_blocknum = curblock;
    for (i = 0; i < DIRENTRIES; i++)
    {
        dirblk->db_fnodes[i].fn_state = FILE_FREE;
    }
    seek(diskfd, curblock);
    write(diskfd, dirblk, sizeof(struct dirblock));

    filetab = dirblk->db_fnodes;
    return OK;
}
