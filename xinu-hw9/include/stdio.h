/**
 * @file stdio.h
 * @provides puts, printf, scanf
 *
 * $Id: stdio.h 181 2007-07-12 19:03:52Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <device.h>
#include <stdarg.h>

#ifndef _STDIO_H_
#define _STDIO_H_

/**
 * \defgroup stdio Standard I/O Library
 * Implements a stripped-down version of standard I/O functions for Xinu.
 * @{
 */
/**
 * Kernel output (for debugging use only)
 */
syscall kprintf(char *, ...);
syscall kputc(device *, unsigned char);
syscall kcheckc(void);
syscall kgetc(void);
syscall kungetc(unsigned char);

#define stdin  CONSOLE
#define stdout CONSOLE
#define stderr CONSOLE

/**
 * Formatted input
 */
int _doscan(register char *, register int **, int (*)(void),
            int (*)(char), int, int);
int fscanf(int, char *, int);
int sscanf(char *, char *, int);
char *fgets(int, char *, int);

/**
 * Formatted output
 */
void _doprnt(char *, va_list, int (*)(int, int), int);
int fprintf(int, char *, ...);
int sprintf(char *, char *, ...);
int fputs(int, char *);

/**
 * CONSOLE input and output
 */
#define puts(s)               fputs(CONSOLE, s)
#define printf(...)      fprintf (CONSOLE, __VA_ARGS__)
#define scanf(fmt, args)      fscanf(CONSOLE, fmt, args)
/**@{*/
#endif                          /* __STDIO_H__ */
