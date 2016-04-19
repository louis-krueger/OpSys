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
    if (sbFreeBlock(supertab, filetab[fd].fn_blocknum) == SYSERR)
    {
	signal(supertab->sb_dirlock);
	return SYSERR;
    }
    filetab[fd].fn_length = 0;
    filetab[fd].fn_cursor = 0;
    filetab[fd].fn_name[0] = '\0';
    free(filetab[fd].fn_data);
    filetab[fd].fn_state = FILE_FREE;
    seek(supertab->sb_disk - devtab, supertab->sb_dirlst->db_blocknum);
    write(supertab->sb_disk - devtab, supertab->sb_dirlst, sizeof(struct dirblock));
    signal(supertab->sb_dirlock);
    return OK;
}
