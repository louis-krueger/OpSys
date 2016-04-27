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

    //if(ether == request)
	//kprintf("xsh_ping (echorequest) ether == request");

    for (i = 0; i < MAX_REQUESTS; i++)
    {
	    attempts++;	    	    
	    // TODO: Zero out memory for receiving packets.	    
	    for (length = 0; length < PKTSZ; length++)
	 	receivepkt[length] = htons(NULL);
	    //  Construct an ICMP echo request packet.  (See icmpPrep() for help
	    if (icmpPrep(request, currpid, dst) == SYSERR)
	    {
	     	//kprintf("xsh_ping.c (echoRequest) icmpPrep(request, currpid, dst) - icmpPrep failed\r\n");
	 	return SYSERR;
	    }
	    ippkt = (struct ipv4gram *)request->data;
	    icmp = (struct icmpgram *)ippkt->data;
	    icmp->type = ICMP_REQUEST;
	    icmp->code = (0x0 << 4);
 	    icmp->cksum = htons(checksum((uchar *)icmp, 16)); //32 estimated, needs to be dynamic
   	    icmp->id = htons(currpid);
	    icmp->seq = htons(i);
	    //ippkt->data[1] = icmp;		    
	    //request->data[1] = ippkt;
	    //  Write the constructed packet to the ethernet device.
	    //kprintf("ping:");rawPrint(request, PKTSZ);//arpPrint(ether, PKTSZ);
	    write(ETH0, request, REQUEST_PKTSZ);
	    //  Read from the ethernet device and sleep. 
	    read(ETH0, ether, PKTSZ);
	    sleep(1000);
	    while((ether->type != htons(ETYPE_IPv4)))
	    {  
		//  Reply to ARP requests using arp_reply() if appropriate.
		if (ether->type == htons(ETYPE_ARP))
	    	{
		    //kprintf("in:");rawPrint(ether, PKTSZ);//arpPrint(ether, PKTSZ);
		    arp = (struct arpgram *)ether->data;
		    if (SYSERR == arp_reply(arp))
		    {
			//kprintf("xsh_ping.c (echoRequest) arp_reply(buf *) - arp_request not for us\r\n");	
		//	return SYSERR;
		    }
		    else
		    {
			arp->htype = 1;
			ether_swap(ether);
			ether->type = htons(ETYPE_ARP);
			ether->data[1] = arp;
			//kprintf("out:");rawPrint(ether, PKTSZ);//arpPrint(request, PKTSZ);
			write(ETH0, ether, ARP_PKTSZ);
			//kprintf("xsh_ping.c (echoRequest) arpReply - reply packet sent\r\n");
	    	    }
	    	}	
	        read(ETH0, ether, PKTSZ);
		sleep(1000);
	    }	    
		//else //if((dst[0] == ether->src[0]) & (dst[1] == ether->src[1])) 
	    	//  Print reply packets (icmpPrint()) and keep stats.
	    	//kprintf("request: ");icmpPrint(request, PKTSZ); 
	 	//kprintf("pong: ");
		icmpPrint(ether->data, PKTSZ);
    		//kprintf("rawether: ");rawPrint(ether, PKTSZ); 
	
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
