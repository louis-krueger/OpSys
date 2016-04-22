/**
 * @file string.h
 *
 * $Id: string.h 176 2007-07-12 01:23:06Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef _STRING_H_
#define _STRING_H_

/**
 * \defgroup string Standard String Library
 * Provides a subset of the standard C string library functions for Xinu.
 * @{
 */
char *strcat(char *, char *);
char *strncat(char *, char *, int);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
int strncmp(char *, char *, int);
char *strcpy(char *, const char *);
char *strncpy(char *, char *, int);
int strlen(const char *);
int strnlen(const char *, ulong);

int memcmp(const void *, const void *, int);
int memcpy(void *, void *, int);
/**@}*/
#endif                          /* __STRING_H__ */
