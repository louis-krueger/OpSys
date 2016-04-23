/**
 * @file getmac.c
 * @provides getmac
 */
/* Embedded Xinu, Copyright (C) 2016.  All rights reserved. */

#include <xinu.h>

/**
 * Retrieve the backend's MAC address.
 *
 * @param *buf pointer to location for MAC address storage
 */
int getmac(void *buf)
{
    char *mac;

    mac = nvramGet("et0macaddr\0");

    if (NULL == mac)
    {
        return SYSERR;
    }

    colon2mac(mac, (uchar *)buf);

    return OK;
}
