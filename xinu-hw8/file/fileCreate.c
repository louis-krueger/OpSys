/* fileCreate.c - fileCreate */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>
#include <memory.h>
#include <string.h>

/*------------------------------------------------------------------------
 * fileCreate - Create a file.
 *------------------------------------------------------------------------
 */
devcall fileCreate(char *name)
{
    int fd = 0;

    if ((NULL == supertab) || (NULL == filetab))
    {
        return SYSERR;
    }

    wait(supertab->sb_dirlock);
    while ((fd < DIRENTRIES) && (FILE_FREE != filetab[fd].fn_state))
    {
        fd++;
    }
    if (fd >= DIRENTRIES)
    {
        signal(supertab->sb_dirlock);
        return SYSERR;
    }

    filetab[fd].fn_length = 0;
    filetab[fd].fn_cursor = 0;
    strncpy(filetab[fd].fn_name, name, FNAMLEN);
    filetab[fd].fn_name[FNAMLEN] = '\0';
    filetab[fd].fn_data = (char *)malloc(DISKBLOCKLEN);
    if (NULL == filetab[fd].fn_data)
    {
        filetab[fd].fn_state = FILE_FREE;
        signal(supertab->sb_dirlock);
        return SYSERR;
    }
    filetab[fd].fn_state = FILE_USED | FILE_OPEN;

    signal(supertab->sb_dirlock);
    filetab[fd].fn_blocknum = sbGetBlock(supertab);
    if (SYSERR == filetab[fd].fn_blocknum)
    {
        return SYSERR;
    }

    return fd;
}
