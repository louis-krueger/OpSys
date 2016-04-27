/**
 * @file icmp.c
 * @provides icmp_reply, icmp_request
 */
/* Embedded Xinu, Copyright (C) 2008, 2016.  All rights reserved. */

#include <xinu.h>

//#define DEBUG
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
#ifdef DEBUG
    struct ethergram *ether = NULL;
    struct ipv4gram *ip = NULL;
    struct icmpgram *icmp = NULL;
    struct arpgram *arp = NULL;

    ether = (struct ethergram *)(buf);
    kprintf("===ethergram===\r\n");
    kprintf("dst: %02X:%02X:%02X:%02X:%02X:%02X\r\n", ether->dst[0], ether->dst[1], ether->dst[2], ether->dst[3], ether->dst[4], ether->dst[5]);
    kprintf("src: %02x:%02X:%02X:%02X:%02X:%02X\r\n", ether->src[0], ether->src[1], ether->src[2], ether->src[3], ether->src[4], ether->src[5]);
   // kprintf("type: %04X\r\n", ether->type);
    kprintf("type: %04X\r\n", ntohs(ether->type));
   // kprintf("type: %04X\r\n", ether->type);
    if (ether->type == ntohs(ETYPE_ARP))
    {
    arp = (struct arpgram *)ether->data;
    if ((arp->oper == ARP_REQUEST) | (arp->oper == ARP_REPLY))
    {
    	kprintf("\t===arpgram===\r\n");
    	kprintf("\tHardware Type: %04X\r\n", arp->htype);
    	kprintf("\tProtocol Type: %04X\r\n", arp->ptype);
    	kprintf("\tHardware addr length: %02X\r\n", arp->hlen);
    	kprintf("\tProtocal addr length: %02X\r\n", arp->plen);
    	kprintf("\tOperation: %04X\r\n", arp->oper);
    	kprintf("\tSender's HW addr: %02X:%02X:%02X:%02X:%02X:%02X\r\n", arp->sha[0], arp->sha[1], arp->sha[2], arp->sha[3], arp->sha[4], arp->sha[5]);
    	kprintf("\tSender's protocol addr: %d.%d.%d.%d\r\n", arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3]);
    	kprintf("\tTarget Hardware addr: %02X:%02X:%02X:%02X:%02X:%02X\r\n", arp->tha[0], arp->tha[1], arp->tha[2], arp->tha[3], arp->tha[4], arp->tha[5]);
    	kprintf("\tTarget protocol addr: %d.%d.%d.%d\r\n", arp->tpa[0], arp->tpa[1], arp->tpa[2], arp->tpa[3]);
    	kprintf("\t==BIG+ENDIAN==\r\n");
    	return OK;
    }
    else
    {
    	kprintf("\t===arpgram===\r\n");
    	kprintf("\tHardware Type: %04X\r\n", ntohs(arp->htype));
    	kprintf("\tProtocol Type: %04X\r\n", ntohs(arp->ptype));
    	kprintf("\tHardware addr length: %02X\r\n", arp->hlen);
    	kprintf("\tProtocal addr length: %02X\r\n", arp->plen);
    	kprintf("\tOperation: %04X\r\n", ntohs(arp->oper));
    	kprintf("\tSender's HW addr: %02X:%02X:%02X:%02X:%02X:%02X\r\n", arp->sha[0], arp->sha[1], arp->sha[2], arp->sha[3], arp->sha[4], arp->sha[5]);
    	kprintf("\tSender's protocol addr: %d.%d.%d.%d\r\n", arp->spa[0], arp->spa[1], arp->spa[2], arp->spa[3]);
    	kprintf("\tTarget Hardware addr: %02X:%02X:%02X:%02X:%02X:%02X\r\n", arp->tha[0], arp->tha[1], arp->tha[2], arp->tha[3], arp->tha[4], arp->tha[5]);
    	kprintf("\tTarget protocol addr: %d.%d.%d.%d\r\n", arp->tpa[0], arp->tpa[1], arp->tpa[2], arp->tpa[3]);
    	kprintf("\t==LIL+ENDIAN==\r\n");
    	return OK;
    }
    }
    ip = (struct ipv4gram *)ether->data;
    kprintf("\t===ipv4gram===\r\n");
    kprintf("\tVersion & Header length : %04X\r\n", ip->ver_hlen);
    kprintf("\tTOS? : %04X\r\n", ip->tos);
    kprintf("\tLength : %04X\r\n", ntohs(ip->length));
    kprintf("\tIdentification : %04X\r\n", ntohs(ip->id));
    kprintf("\tFragment offset : %04X\r\n", ntohs(ip->froff)); 
    kprintf("\tTime To Live : %d\r\n", ip->ttl);
    kprintf("\tProtocol : %04X\r\n", ip->protocol);
    kprintf("\tChecksum : %04X\r\n", ntohs(ip->cksum));
    kprintf("\tSource IP : %d.%d.%d.%d\r\n", ip->src[0], ip->src[1], ip->src[2], ip->src[3]);
    kprintf("\tDest IP : %d.%d.%d.%d\r\n", ip->dst[0], ip->dst[1], ip->dst[2], ip->dst[3]);
    kprintf("\t=============\r\n");

    icmp = (struct icmpgram *)ip->data;
    kprintf("\t\t===icmpgram===\r\n");
    kprintf("\t\tType : %04X\r\n", icmp->type);
    kprintf("\t\tCode : %04X\r\n", icmp->code);
    kprintf("\t\tChecksum : %04X\r\n", ntohs(icmp->cksum));
    kprintf("\t\tID : %04X\r\n", ntohs(icmp->id));
    kprintf("\t\tSequence : %04X\r\n", ntohs(icmp->seq));
    kprintf("\t\t=============\r\n");
#endif  
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
