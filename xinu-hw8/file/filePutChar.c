/* filePutChar.c - filePutChar */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 * filePutChar - Put a byte to a file.
 *------------------------------------------------------------------------
 */
devcall filePutChar(int fd, char c)
{
    if ((NULL == supertab) || (NULL == filetab)
        || (isbadfd(fd)) || (FILE_FREE == filetab[fd].fn_state))
    {
        return SYSERR;
    }

    if ((filetab[fd].fn_cursor < 0) ||
        (filetab[fd].fn_cursor > filetab[fd].fn_length) ||
        (filetab[fd].fn_cursor >= DISKBLOCKLEN))
    {
        return SYSERR;
    }

    filetab[fd].fn_data[filetab[fd].fn_cursor] = c;

    if ((filetab[fd].fn_length < DISKBLOCKLEN) &&
        (filetab[fd].fn_length == filetab[fd].fn_cursor))
    {
        filetab[fd].fn_length++;
    }
    filetab[fd].fn_cursor++;
    filetab[fd].fn_state |= FILE_DIRTY;

    return OK;
}
