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

int arpPrint(void *buf, int length);
int icmpPrep(void *buf, ushort id, char *dst);
int icmpPrint(void *buf, int length);
int rawPrint(void *buf, int length);

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
    struct arpgram *arp;
    int length, i;
    int attempts, totalsent, dropped;
    ushort id, seq;
    bool endLoop;

    attempts = 0;
    //totalsent = 0;
    //dropped = 0;
    //id = 0;
    //seq = 0;
    request = (struct ethergram *)requestpkt;
    ether = (struct ethergram *)receivepkt;
    arp = NULL;
    struct ethergram *epkt = NULL;
    id = currpid;

    if(ether == request)
	kprintf("xsh_ping (echorequest) ether == request");

    for (i = 0; i < MAX_REQUESTS; i++)
    {
	    attempts++;	    	    
	    // TODO: Zero out memory for receiving packets.	    
	    for (length = 0; length < PKTSZ; length++)
	 	receivepkt[length] = htons(NULL);
	    //  Construct an ICMP echo request packet.  (See icmpPrep() for help
	    //ippkt->data[1] = icmp;		    
	    //icmp->type = htons(ICMP_REQUEST);
	    //icmp->code = NULL;
	    //icmp->cksum = NULL;
   	    //icmp->id = 1337;
	    //icmp->seq = htons(i);
	    //ippkt->data[1] = icmp;
	    //request->data[1] = ippkt;
	    if (icmpPrep(request, id, dst) == SYSERR)
	    {
	     	kprintf("xsh_ping.c (echoRequest) echoRequest - icmpPrep failed\r\n");
	 	return SYSERR;
	    }
	    //  Write the constructed packet to the ethernet device.
	    write(ETH0, request, PKTSZ);
	    //  Read from the ethernet device and sleep.
	    read(ETH0, ether, PKTSZ);
	    sleep(1000);
	    //  Reply to ARP requests using arp_reply() if appropriate.
	    //icmp = (struct icmpgram *) ((struct ipv4gram *) ether->data)->data;
	    //icmp->seq = htons(icmp->seq);
	    if (ether->type == htons(ETYPE_ARP))
	    {
		arp = (struct arpgram *)ether->data;
		kprintf("in:");rawPrint(ether, PKTSZ);//arpPrint(ether, PKTSZ);
	        memcpy(request->dst, ether->src, ETH_ADDR_LEN);
		getmac(request->src); 
		request->type = htons(ETYPE_ARP);
		arp_reply(arp);
		request->data[1] = arp; 
		kprintf("out:");rawPrint(request, PKTSZ);//arpPrint(request, PKTSZ);
		write(ETH0, request, PKTSZ);
		kprintf("xsh_ping.c (echoRequest) arpReply - reply packet sent\r\n");
	    }
	    //  Print reply packets (icmpPrint()) and keep stats.
	    //kprintf("request: ");icmpPrint(request, PKTSZ);
	    //kprintf("ether: ");icmpPrint(ether, PKTSZ);
    } 
  
    kprintf("****end of echo request attempts: %02d****\r\n", attempts);
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
