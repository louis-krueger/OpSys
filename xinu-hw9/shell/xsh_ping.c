/**
 * @file     xsh_ping.c
 * @provides xsh_ping, echoRequest
 *  modified by
 * Louis Krueger
 * &
 * Samuel Scheel
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2009, 2016.  All rights reserved. */

#include <xinu.h>

int icmpPrep(void *buf, ushort id, char *dst);
int icmpPrint(void *buf, int length);

/**
 * Provides a client that sends out a series of ping requests.
 * @param dst Dotted decimal destination string.
 * @return OK for success, SYSERR for syntax error
 */
int echoRequest(char *dst)
{
    char requestpkt[REQUEST_PKTSZ];
    char receivepkt[PKTSZ];
    struct ethergram *ether, *request;
    struct ipv4gram *ippkt;
    struct icmpgram *icmp;
    int length, i;
    int attempts, totalsent, dropped;
    ushort id, seq;
    bool endLoop;

    //totalsent = 0;
    //dropped = 0;
    //id = 0;
    //seq = 0;
    request = (struct ethergram *)requestpkt;
    ether = (struct ethergram *)receivepkt;
    struct ethergram *epkt = NULL;
    id = 1;

    if(ether == request)
	kprintf("xsh_ping (echorequest) ether == request");

    netInit();
    for (i = 0; i < MAX_REQUESTS; i++)
	    {
	    
	    // TODO: Zero out memory for receiving packets.
	    
	    for (length = 0; length < PKTSZ; length++)
		receivepkt[length] = NULL;
	
	
	    //  Construct an ICMP echo request packet.  (See icmpPrep() for help)
	    	/* Assign the etherPkt to point to the passed buffer */
	    epkt = (struct ethergram *)ether;
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
	    //dot2ip(nvramGet("lan_ipaddr\0"), ippkt->src);
	    //dot2ip(dst, ippkt->dst);
	    //ippkt->cksum = checksum((uchar *)ippkt,
	    //                       (4 * (ippkt->ver_hlen & IP_IHL)));
	    	/* Set up the ethergram portion of packet */
	    //getmac(epkt->src);
	
	
	    //  Write the constructed packet to the ethernet device.
	    write(ETH0, ippkt, PKTSZ);
	    //  Read from the ethernet device and sleep.
	    read(ETH0, ether, PKTSZ);
	    sleep(1000);
	    //icmpPrint(ether, PKTSZ);
	    //  Reply to ARP requests using arp_reply() if appropriate.
	     
	    //if ARP request reply
	    //	 
	
	    //  Print reply packets (icmpPrint()) and keep stats.
	    icmpPrint(ether, PKTSZ);
    } 
  
    kprintf("****end of echo request****\r\n");
    return OK;
}


/**
 * Shell command (ping) sends echo requests to provided IP.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_ping(ushort nargs, char *args[])
{
    /* Output help, if '--help' argument was supplied */
    if ((nargs == 2 && strncmp(args[1], "--help", 6) == 0) || (nargs < 2))
    {
        printf("Usage: ping [IP address]\n");
        printf("Sends echo requests to provided IP address.\n");
        printf("\t--help\t display this help and exit\n");

        return OK;
    }

    echoRequest(args[1]);

    return OK;
}
/* Decipher the reply to the echo request
 * @param *buf pointer to the ethernet pkt
 * @param length length of ethernet pkt
 *  
 *
 */
int icmpPrint(void *buf, int length)
{
    struct ipv4gram *ip = NULL;
    struct icmpgram *icmp = NULL;

    ip = (struct ipv4gram *)buf;
    icmp = (struct icmpgram *)ip->data;

    printf("%d bytes from ", ntohs(ip->length));
    printf("(%d.%d.%d.%d) ",
           ip->src[0], ip->src[1], ip->src[2], ip->src[3]);
    printf("icmp_seq=%d ", ntohs(icmp->seq));
    printf("ttl=%d", ip->ttl);
    if (ICMP_REPLY != icmp->type)
    {
        printf("\t(not reply)");
    }

    printf("\n");
    return OK;
}



