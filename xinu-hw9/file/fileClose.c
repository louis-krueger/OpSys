/* fileClose.c - fileClose */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileClose - Close a file.
 *------------------------------------------------------------------------
 */
devcall fileClose(int fd)
{
    int result;
    if ((NULL == supertab) || (NULL == filetab)
        || (isbadfd(fd)) || (FILE_FREE == filetab[fd].fn_state))
    {
        return SYSERR;
    }

    if (FILE_DIRTY & filetab[fd].fn_state)
    {
        seek(DISK0, filetab[fd].fn_blocknum);
        write(DISK0, filetab[fd].fn_data, DISKBLOCKLEN);
        filetab[fd].fn_state &= ~FILE_DIRTY;
    }
    filetab[fd].fn_state &= ~FILE_OPEN;

    wait(supertab->sb_dirlock);
    seek(DISK0, supertab->sb_dirlst->db_blocknum);
    result = write(DISK0, supertab->sb_dirlst, sizeof(struct dirblock));
    signal(supertab->sb_dirlock);

    if (SYSERR == result)
    {
        return SYSERR;
    }
    return OK;
}
