/* Copyright (C) 2007, Marquette University.  All rights reserved. */
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BLOCKSIZE 256
#define BLOCKNUM  256
#define MAXLINE 256
#define MAXTRIES 10

#define XINU_QUERY0 "XINU disk daemon?\r\n"
#define XINU_QUERY1 "XINU disk daemon?\n\r"
#define XINU_QUERY2 "XINU disk daemon?\r"
#define XINU_QUERY3 "XINU disk daemon?\n"

char decode(char *buffer)
{
    char c1, c2;
    int n;

    c1 = buffer[0];
    c2 = buffer[1];

    if ((c1 >= '0') && (c1 <= '9'))
    {
        c1 -= '0';
    }
    else
    {
        c1 = 10 + c1 - 'A';
    }
    if ((c2 >= '0') && (c2 <= '9'))
    {
        c2 -= '0';
    }
    else
    {
        c2 = 10 + c2 - 'A';
    }
    return (char)c1 *16 + c2;
}

char encode(char *buffer, char b)
{
    buffer[0] = (b & 0xF0) >> 4;
    buffer[1] = b & 0x0F;
    if (buffer[0] > 9)
        buffer[0] = 'A' + (buffer[0] - 10);
    else
        buffer[0] += '0';
    if (buffer[1] > 9)
        buffer[1] = 'A' + (buffer[1] - 10);
    else
        buffer[1] += '0';
    return 0;
}

int readBlockRequest(FILE * datafile, FILE * infile, FILE * outfile,
                     int block)
{
    int i;
    char buffer[BLOCKSIZE * 2 + 1];
    char buffin[BLOCKSIZE];
    unsigned char chksum = 0;
    printf("Read  block %4d\n", block);
    fseek(datafile, block * BLOCKSIZE, SEEK_SET);
    // printf(" file offset %6d,", ftell(datafile));
    fread(buffin, 1, BLOCKSIZE, datafile);
    for (i = 0; i < BLOCKSIZE; i++)
    {
        encode(&buffer[i * 2], buffin[i]);
        chksum ^= buffin[i];
    }
    buffer[BLOCKSIZE * 2] = '\0';
    fprintf(outfile, " %s %02X\n", buffer, chksum & 0x00FF);
    // printf(" chksum %02X\n", chksum & 0x00FF);
    return 0;
}

int writeBlockRequest(FILE * datafile, FILE * infile, FILE * outfile,
                      int block)
{
    int i;
    char c;
    char buffer[BLOCKSIZE * 2 + 1];
    unsigned char chksum = 0;
    printf("Write block %4d\n", block);
    fgets(buffer, BLOCKSIZE * 2 + 1, infile);
    for (i = 0; i < BLOCKSIZE; i++)
    {
        buffer[i] = decode(&buffer[i * 2]);
        chksum ^= buffer[i];
    }
    buffer[BLOCKSIZE] = '\0';
    i = fseek(datafile, block * BLOCKSIZE, SEEK_SET);
    // printf(" file offset %6d,", ftell(datafile));
    i = fwrite(buffer, BLOCKSIZE, 1, datafile);
    fprintf(outfile, " %02X\n", chksum & 0x00FF);
    // printf(" chksum %02X\n", chksum & 0x00FF);
    return 0;
}

void diskServer(int infd, int outfd, FILE * datafile)
{
    int n, tries;
    char line[MAXLINE];
    int block;
    char command;
    FILE *infile;
    FILE *outfile;

    infile = fdopen(infd, "r");
    outfile = fdopen(outfd, "w");

    for (tries = 0; tries < MAXTRIES; tries++)
    {
        fprintf(outfile, "XINU Disk Daemon.\n");
        fflush(outfile);
        fgets(line, MAXLINE, infile);
        //      n = write(outfd, "XINU Disk Daemon.\n", 18);
        //      n = read(infd, line, MAXLINE);
        //      line[n] = '\0';
        if (0 == strncmp(line, XINU_QUERY0, MAXLINE))
        {
            printf("Contacted XINU disk driver.\n");
            break;
        }
        else if (0 == strncmp(line, XINU_QUERY1, MAXLINE))
        {
            printf
                ("Contacted XINU disk driver, but output is overcooked:\n");
            printf("\tExpected line to end with \\r\\n, but got \\n\\r.");
            break;
        }
        else if (0 == strncmp(line, XINU_QUERY2, MAXLINE))
        {
            printf
                ("Contacted XINU disk driver, but output is undercooked:\n");
            printf
                ("\tExpected line to end with \\r\\n, but got \\r only.");
            break;
        }
        else if (0 == strncmp(line, XINU_QUERY3, MAXLINE))
        {
            printf
                ("Contacted XINU disk driver, but output is undercooked:\n");
            printf
                ("\tExpected line to end with \\r\\n, but got \\n only.");
            break;
        }
        sleep(1);
    }

    if (tries >= MAXTRIES)
        return;

    printf("ACK\n");
    //  n = write(outfd, "ACK\r\n", 5);
    fprintf(outfile, "ACK\n");
    fflush(outfile);

    while (1)
    {
        fgets(line, MAXLINE, infile);
        // n = read(infd, line, MAXLINE);
        if (2 == sscanf(line, "%c %02X\n", &command, &block))
        {
            if ('R' == command)
            {
                readBlockRequest(datafile, infile, outfile, block);
            }

            if ('W' == command)
            {
                writeBlockRequest(datafile, infile, outfile, block);
            }

        }
        else if (0 == strncmp(line, XINU_QUERY0, MAXLINE))
        {
            printf("Contacted XINU disk driver.\n");
            sleep(1);
            fprintf(outfile, "ACK\n");
        }
        fflush(outfile);
        fflush(datafile);
    }
}

/**
 * This program sets up a pipe, forks a child, and closes the appropriate
 *  descriptors for a one-way pipe from the parent to the child.
 * The parent then sends a string down the pipe, which the child reads
 *  from the pipe and then prints to stdout.
 */

int main(int argc, char **argv)
{
    pid_t pid;
    int n = 0, tries = 0;

    int inpipefd[2];
    int outpipefd[2];

    FILE *datafile = NULL;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <file> <backend>\n", argv[0]);
        exit(-1);
    }

    printf("Opening disk file %s.\n", argv[1]);
    datafile = fopen(argv[1], "r+");
    if (NULL != datafile)
    {
        n = fseek(datafile, 0L, SEEK_END);
        n = ftell(datafile);
        printf("Max disk block is %d.\n", n);
    }
    if ((NULL == datafile) || (n < BLOCKSIZE * BLOCKNUM - 1))
    {
        if (NULL != datafile)
        {
            fclose(datafile);
        }
        printf("Reopening disk file %s\n", argv[1]);
        if (NULL == (datafile = fopen(argv[1], "a+")))
        {
            fprintf(stderr, "failed to open file %s\n", argv[1]);
            exit(-1);
        }
        fseek(datafile, 0L, SEEK_SET);
        for (; n < BLOCKSIZE * BLOCKNUM - 1; n++)
        {
            fputc(0, datafile);
        }
        fclose(datafile);
        if (NULL == (datafile = fopen(argv[1], "r+")))
        {
            fprintf(stderr, "failed to reopen file %s\n", argv[1]);
            exit(-1);
        }
    }


    if ((pipe(inpipefd) < 0) || (pipe(outpipefd) < 0))  /* Create a pipe. */
    {
        fprintf(stderr, "Pipe error!\n");
        exit(-1);
    }

    pid = fork();               /* fork another process */

    if (pid < 0)                /* Forking error! */
    {
        exit(-1);
    }
    else if (0 == pid)          /* Child process */
    {
        close(outpipefd[1]);    /* Child closes write end of out pipe. */
        dup2(outpipefd[0], STDIN_FILENO);
        /* Hook read end of out pipe to stdin. */
        close(outpipefd[0]);    /* Clean up duplicate descriptor.  */
        close(inpipefd[0]);     /* Child closes read end of in pipe. */
        dup2(inpipefd[1], STDOUT_FILENO);
        /* Hook write end of in pipe to stdout. */
        close(inpipefd[1]);     /* Clean up duplicate descriptor.  */

        /* Replace the child process image with the "more" command. */
        execlp("xinu-console", "xinu-console", argv[2], NULL);
        /* Should never get past this line if execlp succeeded. */
    }
    else                        /* pid > 0: Parent process */
    {
        close(outpipefd[0]);    /* Parent closes read end of out pipe. */
        close(inpipefd[1]);     /* Parent closes write end of in pipe. */

        diskServer(inpipefd[0], outpipefd[1], datafile);

        write(outpipefd[1], "\0", 1);
        write(outpipefd[1], "q\n", 2);
        wait(NULL);
        close(outpipefd[1]);    /* Done with pipe. */
        close(inpipefd[0]);
        fclose(datafile);
        exit(0);
    }
}
