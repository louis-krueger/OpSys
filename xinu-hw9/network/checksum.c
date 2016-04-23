/**
 * @file checksum.c
 * @provides checksum
 */
/* Embedded Xinu, Copyright (C) 2016.  All rights reserved. */

#include <xinu.h>


/**
 * Perform a standard checksum on the data. (From RFC 1071.)
 *
 * @param *addr address to begin checksum on
 * @param length number of bytes to checksum
 * @return checksum value
 */
ushort checksum(void *addr, int length)
{
    register ulong sum = 0;
    ushort *ptr = addr;

    while (length > 1)
    {
        sum += *ptr++;
        length -= 2;
    }

    /* Add left-over byte, if any */
    if (length > 0)
    {
        sum += *((uchar *)ptr);
    }

    /* Fold 32-bit sum to 16 bits */
    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}
