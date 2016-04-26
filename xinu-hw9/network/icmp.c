/**
 * @file icmp.c
 * @provides icmp_reply, icmp_request
 */
/* Embedded Xinu, Copyright (C) 2008, 2016.  All rights reserved. */

#include <xinu.h>

#define NARP_ENT 27
char *arp_map[NARP_ENT][2] = {
    {"192.168.6.10", "52:54:03:02:B1:06"},      // morbius
    {"192.168.6.11", "00:15:17:D2:62:EE"},      // eldrad
    {"192.168.6.12", "00:10:18:4E:5A:0F"},      // mawdryn
    {"192.168.6.101", "00:16:B6:28:7D:4F"},     // voc
    {"192.168.6.102", "00:16:B6:28:7D:52"},     // supervoc
    {"192.168.6.103", "00:18:39:6f:78:15"},     // cass
    {"192.168.6.104", "00:18:39:6f:78:18"},     // d84
    {"192.168.6.105", "00:18:39:6f:78:1e"},     // uvanov
    {"192.168.6.106", "00:18:39:6f:78:12"},     // poul
    {"192.168.6.107", "00:18:39:6f:78:0f"},     // toos
    {"192.168.6.108", "00:1d:7e:53:8f:11"},     // gravis
    {"192.168.6.109", "00:18:39:6f:78:06"},     // zilda
    {"192.168.6.110", "00:1d:7e:53:ad:77"},     // chub
    {"192.168.6.111", "00:1d:7e:53:ad:a7"},     // sutekh
    {"192.168.6.112", "00:1d:7e:53:8d:85"},     // mandragora
    {"192.168.6.113", "00:1d:7e:53:ad:71"},     // xoanon
    {"192.168.6.114", "00:1d:7e:53:8e:d5"},     // mentalis
    {"192.168.6.115", "00:1d:7e:53:a2:a3"},     // fendahl
    {"192.168.6.116", "00:1d:7e:53:ad:ef"},     // meglos
    {"192.168.6.117", "00:1d:7e:3f:dc:d9"},     // adrasta
    {"192.168.6.118", "00:1d:7e:3f:df:79"},     // davros
    {"192.168.6.119", "00:1d:7e:3f:d8:56"},     // melkur
    {"192.168.6.120", "00:1d:7e:3f:d8:62"},     // nimon
    {"192.168.6.121", "00:1e:e5:85:ed:c5"},     // malus
    {"192.168.6.122", "00:1e:e5:86:02:7a"},     // azal
    {"192.168.6.123", "00:1e:e5:86:06:6a"},     // fenric
    {"192.168.6.125", "00:18:39:6f:78:03"}      // dask
};


/**
 * Construct the IP and Ethernet portions of an ICMP packet.
 * @param *buf pointer to the ethernet pkt
 * @param *dst dotted decimal destination address
 */
int icmpPrep(void *buf, ushort id, char *dst)
{
    struct ethergram *epkt = NULL;
    struct ipv4gram *ippkt = NULL;

    /* Assign the etherPkt to point to the passed buffer */
    epkt = (struct ethergram *)buf;
    ippkt = (struct ipv4gram *)epkt->data;

    /* Set up the ipv4gram portion of packet */
    ippkt->ver_hlen = (IP_V4 << 4);
    ippkt->ver_hlen += (IPv4_SIZE / 4);
    ippkt->tos = 0;
    ippkt->length = htons(REQUEST_PKTSZ - ETHER_SIZE);
    ippkt->id = htons(id);
    ippkt->froff = (IP_FLAG_DF << 13);
    ippkt->froff += 0;
    ippkt->froff = htons(ippkt->froff);
    ippkt->ttl = IP_TTL;
    ippkt->protocol = IP_ICMP;
    ippkt->cksum = 0;
    dot2ip(nvramGet("lan_ipaddr\0"), ippkt->src);
    dot2ip(dst, ippkt->dst);
    ippkt->cksum = checksum((uchar *)ippkt,
                            (4 * (ippkt->ver_hlen & IP_IHL)));


    /* Set up the ethergram portion of packet */
    getmac(epkt->src);

    int i;
    /* Find MAC address for IP destinations on the local XinuNet */
    for (i = 0; i < NARP_ENT; i++)
    {
        if (0 == strncmp(dst, arp_map[i][0], 16))
        {
            colon2mac(arp_map[i][1], epkt->dst);
            break;
        }
    }

    if (NARP_ENT == i)
    {
        colon2mac(PING_GATEWAY, epkt->dst);
    }
    epkt->type = htons(ETYPE_IPv4);

    return OK;
}

/**
 * Decipher the reply to the echo request
 * @param *buf pointer to the ethernet pkt
 * @param length length of ethernet pkt
 */
int rawPrint(void *buf, int length)
{
    struct ethergram *ether = NULL;
    struct ipv4gram *ip = NULL;
    struct icmpgram *icmp = NULL;
    struct arpgram *arp = NULL;

    ether = (struct ethergram *)buf;
    kprintf("===ethergram===\r\n");
    kprintf("dst: %08X\r\n", (long)ether->dst);
    kprintf("src: %08X\r\n", (long)ether->src);
    kprintf("type: %08X\r\n", ether->type);
    if (ntohs(ether->type) == ETYPE_ARP)
    {
        kprintf("\t===arpgram===\r\n");
	arp = (struct arpgram *)ether->data;
	kprintf("\t arp degub here\r\n");
	kprintf("\t=============\r\n");
	return OK;
    }
    ip = (struct ipv4gram *)ether->data;
    kprintf("\t===ipv4gram===\r\n");
    kprintf("\tdst: %08X\r\n", (long)ip->dst);
    kprintf("\tsrc: %08X\r\n", (long)ip->src);
    kprintf("\tttl: %08X\r\n", ip->ttl);

    icmp = (struct icmpgram *)ip->data;
    kprintf("\t===icmpgram===\r\n");
    kprintf("\ttype: %08X\r\n", (long)icmp->type);
    kprintf("\tcode: %08X\r\n", (long)icmp->code);
    kprintf("\tseq %08X\r\n", icmp->seq);
  
    return OK;	
}
int arpPrint(void *buf, int length)
{
    struct ethergram *ether = NULL;
    struct arpgram *arp = NULL;

    ether = (struct ethergram *)buf;
    arp = (struct arpgram *)ether->data;
    kprintf("\r\n======arpGram============\r\n");
    kprintf("from: (%x:%x:%x:%x:%x:%x) \r\n",
           ether->src[2], ether->src[3], ether->src[4], ether->src[5], ether->src[6], ether->src[7]);
    kprintf("tpye[htype, ptype]: %08X, %08X\r\n", ntohs(arp->htype), ntohs(arp->ptype));
    kprintf("ether type: %08X\r\n", ntohs(ether->type));
    if (ARP_REPLY != ether->type)
    {
        kprintf("\t(not reply)\r\n");
    }
    kprintf("======================\r\n");
    kprintf("\n");
    return OK;
}
int icmpPrint(void *buf, int length)
{
    struct ipv4gram *ip = NULL;
    struct icmpgram *icmp = NULL;

    ip = (struct ipv4gram *)buf;
    icmp = (struct icmpgram *)ip->data;

    kprintf("%d bytes from ", ntohs(ip->length));
    kprintf("(%d.%d.%d.%d) ",
           ip->src[0], ip->src[1], ip->src[2], ip->src[3]);
    kprintf("icmp_seq=%d ", ntohs(icmp->seq));
    kprintf("ttl=%d", ip->ttl);
    if (ICMP_REPLY != icmp->type)
    {
        printf("\t(not reply)\r\n");
    }

    kprintf("\r\n");
    return OK;
}
