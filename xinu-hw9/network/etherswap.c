/**
 * @file etherswap.c
 * @provides ether_swap
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Swap the source/destination MAC addresses 
 * (well the source address is forced to be ours)
 *
 * @param *buf pointer to the ethernet pkt
 */
int ether_swap(void *buf)
{
    struct ethergram *gram = (struct ethergram *)buf;
    uchar temp[ETH_ADDR_LEN];

    if (SYSERR == getmac(temp))
    {
        return SYSERR;
    }
    //colon2mac(nvramGet("et0macaddr\0"), temp);

    memcpy(gram->dst, gram->src, ETH_ADDR_LEN);
    memcpy(gram->src, temp, ETH_ADDR_LEN);

    return OK;
}
