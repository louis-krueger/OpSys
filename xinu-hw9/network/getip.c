/**
 * @file getip.c
 * @provides getip
 */
/* Embedded Xinu, Copyright (C) 2016.  All rights reserved. */

#include <xinu.h>

/**
 * Retrieve the backend's IP address.
 *
 * @param *buf pointer to location for IP address storage
 */
int getip(void *buf)
{
    char *ip;

    ip = nvramGet("lan_ipaddr\0");

    if (NULL == ip)
    {
        return SYSERR;
    }

    dot2ip(ip, (uchar *)buf);

    return OK;
}
