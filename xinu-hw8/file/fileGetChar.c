/* fileGetChar.c - fileGetChar */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileGetChar - Get a byte from a file.
 *------------------------------------------------------------------------
 */
devcall fileGetChar(int fd)
{
    char c;
    if ((NULL == supertab) || (NULL == filetab)
        || (isbadfd(fd)) || (FILE_FREE == filetab[fd].fn_state))
    {
        return SYSERR;
    }

    if ((filetab[fd].fn_cursor < 0) ||
        (filetab[fd].fn_cursor >= filetab[fd].fn_length))
    {
        return SYSERR;
    }

    c = filetab[fd].fn_data[filetab[fd].fn_cursor];
    if (filetab[fd].fn_cursor < filetab[fd].fn_length)
    {
        filetab[fd].fn_cursor++;
    }

    return c;
}
