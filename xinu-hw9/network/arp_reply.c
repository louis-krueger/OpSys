/**
 * @file arp_reply.c
 * @provides arp_reply
 */
/* Embedded Xinu, Copyright (C) 2016.  All rights reserved. */

#include <xinu.h>

/**
 * Swap and complete an arp request.
 *
 * @param *buf pointer to the ethernet pkt
 */
int arp_reply(void *buf)
{
    struct arpgram *gram = (struct arpgram *)buf;
    uchar enet[ETH_ADDR_LEN];
    uchar ip[IP_ADDR_LEN];

    if (SYSERR == getmac(enet))
    {
        return SYSERR;
    }

    if (SYSERR == getip(ip))
    {
        return SYSERR;
    }

    if (0 == strncmp((char *)ip, (char *)gram->tpa, IP_ADDR_LEN))
    {
        gram->oper = htons(ARP_REPLY);

        memcpy(gram->tha, gram->sha, ETH_ADDR_LEN);
        memcpy(gram->tpa, gram->spa, IP_ADDR_LEN);
        memcpy(gram->sha, enet, ETH_ADDR_LEN);
        memcpy(gram->spa, ip, IP_ADDR_LEN);
    }
    else
    {
        return SYSERR;
    }

    return OK;
}
