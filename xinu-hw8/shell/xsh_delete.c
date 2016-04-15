/**
 * @file     xsh_delete.c
 * @provides xsh_delete
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (delete) deletes existing files.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_delete(int nargs, char *args[])
{
    int fd = 0;
    char c = 0;

    /* Output help, if '--help' argument was supplied */
    if ((nargs == 2 && strncmp(args[1], "--help", 6) == 0)
        || (nargs != 2))
    {
        printf("Usage: delete [filename]\n");
        printf("Delete a file from disk.\n");
        printf("\t--help\t display this help and exit\n");

        return OK;
    }
    fd = fileOpen(args[1]);
    if (SYSERR == fd)
    {
        printf("File \"%s\" not found.\n", args[1]);
        return OK;
    }
    fileDelete(fd);

    return OK;
}
