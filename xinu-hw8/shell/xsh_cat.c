/**
 * @file     xsh_cat.c
 * @provides xsh_cat
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command () prints existing files or creates new ones.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_cat(int nargs, char *args[])
{
    int fd = 0;
    char c = 0;

    /* Output help, if '--help' argument was supplied */
    if ((nargs == 2 && strncmp(args[1], "--help", 6) == 0) || (nargs > 3)
        || (nargs < 2))
    {
        printf("Usage: cat [filename] or cat > [filename]\n");
        printf
            ("Print file contents or copy console input into new file.\n");
        printf("\t--help\t display this help and exit\n");

        return OK;
    }
    if (nargs == 2)
    {                           /* Cat a file */
        fd = fileOpen(args[1]);
        if (SYSERR != fd)
        {
            fileSeek(fd, 0);
            while ((c = fileGetChar(fd)) != (char)SYSERR)
            {
                printf("%c", c);
            }
        }
        else
        {
            printf("File \"%s\" not found.\n", args[1]);
        }
        printf("\n\n");
        return OK;
    }
    if (strncmp(args[1], ">", 2))
    {
        printf("ERROR: Cannot read from \"%s\", only CONSOLE!\n",
               args[1]);
        return SYSERR;
    }
    if (SYSERR != fileOpen(args[2]))
    {
        printf("ERROR: file \"%s\" already exists!\n", args[2]);
        return SYSERR;
    }
    fd = fileCreate(args[2]);
    if (SYSERR == fd)
    {
        printf("ERROR creating file \"%s\"\n", args[2]);
        return SYSERR;
    }
    printf("Creating new file \"%s\".  Enter ~ to exit.\n", args[2]);
    while ((c = getc(CONSOLE)) != '~')
    {
        filePutChar(fd, c);
    }
    fileClose(fd);

    return OK;
}
