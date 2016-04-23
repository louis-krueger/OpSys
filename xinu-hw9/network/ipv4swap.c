/**
 * @file ipv4swap.c
 * @provides ipv4_swap
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Swap the IPv4 header data.
 *
 * @param *buf pointer to the ethernet pkt
 */
int ipv4_swap(void *buf, int length)
{
    struct ipv4gram *gram = (struct ipv4gram *)buf;
    char temp[IP_ADDR_LEN];

    gram->cksum = htons(0x0000);

    memcpy(temp, gram->dst, IP_ADDR_LEN);
    memcpy(gram->dst, gram->src, IP_ADDR_LEN);
    memcpy(gram->src, temp, IP_ADDR_LEN);

    gram->cksum = checksum(buf, length);

    return OK;
}
