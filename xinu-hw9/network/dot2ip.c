/**
 * @file dot2ip.c
 * @provides dot2ip
 */
/* Embedded Xinu, Copyright (C) 2016.  All rights reserved. */

#include <xinu.h>

/**
 * Convert a period separated string representation of an IP into
 *  the equivalent byte array.
 * @param src pointer to dot-separated IP string
 * @param dst pointer to byte array
 * @return number of octets converted.
 */
int dot2ip(char *src, uchar *dst)
{
    uchar count = 0, c = 0;
    ulong temp = 0;
    ulong ipaddr = 0;
    if (NULL == src || NULL == dst)
    {
        return SYSERR;
    }

    count = 0;

    while ((count < IP_ADDR_LEN) && *src)
    {
        c = *src;
        if (isdigit(c))
        {
            temp = temp * 10 + (c - '0');
        }
        else if ('.' == c)
        {
            ipaddr += temp;
            ipaddr = ipaddr << 8;
            temp = 0;
            count++;
        }
        else
        {
            return SYSERR;
        }
        src++;
    }
    ipaddr += temp;

    ipaddr = htonl(ipaddr);
    memcpy(dst, &ipaddr, 4);

    return OK;
}
