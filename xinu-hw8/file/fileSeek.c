/* fileSeek.c - fileSeek */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileSeek - Seek to a new offset within a file.
 *------------------------------------------------------------------------
 */
devcall fileSeek(int fd, int offset)
{
    if ((NULL == supertab) || (NULL == filetab)
        || (isbadfd(fd)) || (FILE_FREE == filetab[fd].fn_state))
    {
        return SYSERR;
    }

    if ((filetab[fd].fn_cursor < 0) ||
        (filetab[fd].fn_cursor > filetab[fd].fn_length) ||
        (filetab[fd].fn_cursor > DISKBLOCKLEN))
    {
        return SYSERR;
    }

    if ((offset < 0) ||
        (offset > filetab[fd].fn_length) || (offset >= DISKBLOCKLEN))
    {
        return SYSERR;
    }

    filetab[fd].fn_cursor = offset;
    return OK;
}
