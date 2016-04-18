/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/* Samuel Scheel                                                   */
/* and                                                             */
/* Louis Krueger                                                   */
/* TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.
    if ((isbadfd(fd)) || (NULL == supertab) || (NULL == filetab))
    {
        return SYSERR;
    }
    wait(supertab->sb_dirlock);
    kprintf("filetab[fd].fn_blocknum = %d\r\n", filetab[fd].fn_blocknum);
    if (sbFreeBlock(supertab, filetab[fd].fn_blocknum) == SYSERR)
    {
	signal(supertab->sb_dirlock);
	return SYSERR;
    }
    kprintf("1 fn_state = %d\r\n", filetab[fd].fn_state);
    filetab[fd].fn_state = FILE_FREE;
    kprintf("2 fn_state = %d\r\n", filetab[fd].fn_state);
    signal(supertab->sb_dirlock);
    return OK;
}
