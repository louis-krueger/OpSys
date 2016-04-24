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

    if(ether == request)
	kprintf("xsh_ping (echorequest) ether == request");
    
    // TODO: Zero out memory for receiving packets.
    for (i = 0; i  < PKTSZ; i++)
	receivepkt[i] = NULL;
    //if(SYSERR == icmpPrep(ether, 100, 1));
    //	kprintf("xsh_ping (echoRequest) SYSERR 1");	
    

    //  Construct an ICMP echo request packet.  (See icmpPrep() for help)

    //  Write the constructed packet to the ethernet device.
    //  Read from the ethernet device and sleep.
    read(ETH0, receivepkt, PKTSZ);
    read(ETH0, request, PKTSZ);
    read(ETH0, ether, PKTSZ);
    sleep(1000);
    //icmpPrint(ether, PKTSZ);
    //  Reply to ARP requests using arp_reply() if appropriate.
    //  Print reply packets (icmpPrint()) and keep stats.

    icmpPrint(receivepkt, PKTSZ);
    icmpPrint(request, PKTSZ);
    icmpPrint(ether, PKTSZ);
   
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

