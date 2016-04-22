/* fileOpen.c - fileOpen */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>
#include <memory.h>
#include <string.h>

/*------------------------------------------------------------------------
 * fileOpen - Open a file, return a file descriptor.
 *------------------------------------------------------------------------
 */
devcall fileOpen(char *name)
{
    int fd = 0;
    int curblock;

    // A little error checking.
    if ((NULL == supertab) || (NULL == filetab))
    {
        return SYSERR;
    }

    // Run through the file descriptors, looking for a match.
    for (fd = 0; fd < DIRENTRIES; fd++)
    {
        if ((FILE_USED & filetab[fd].fn_state)
            && (0 == strncmp(filetab[fd].fn_name, name, FNAMLEN)))
        {
            // Check to see if file is already open.
            if (FILE_OPEN & filetab[fd].fn_state)
            {
                return fd;
            }
            // Open file by reading disk block into memory.
            curblock = (int)filetab[fd].fn_blocknum;
            filetab[fd].fn_data = (char *)malloc(DISKBLOCKLEN);
            if (NULL == filetab[fd].fn_data)
            {
                return SYSERR;
            }
            seek(DISK0, curblock);
            read(DISK0, filetab[fd].fn_data, DISKBLOCKLEN);
            filetab[fd].fn_state |= FILE_OPEN;
            filetab[fd].fn_cursor = 0;
            // Return the descriptor for this file.
            return fd;
        }
    }
    return SYSERR;
}
