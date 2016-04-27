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
    totalsent = 0;
    dropped = 0;
    id = 0;
    seq = 0;
    request = (struct ethergram *)requestpkt;
    ether = (struct ethergram *)receivepkt;

    //if(ether == request)
	//kprintf("xsh_ping (echorequest) ether == request");

    for (i = 0; i < MAX_REQUESTS; i++)
    {
	    	    	    
	    // TODO: Zero out memory for receiving packets.	    
	    bzero(requestpkt, REQUEST_PKTSZ);
	    bzero(receivepkt, PKTSZ);
	    //  Construct an ICMP echo request packet.  (See icmpPrep() for help
	    if (icmpPrep(request, currpid, dst) == SYSERR)
	    {
	     	//kprintf("xsh_ping.c (echoRequest) icmpPrep(request, currpid, dst) - icmpPrep failed\r\n");
	 	return SYSERR;
	    }
	    ippkt = (struct ipv4gram *)request->data;
	    icmp = (struct icmpgram *)ippkt->data;
	    icmp->type = ICMP_REQUEST;
	    icmp->code = 0x0;
   	    icmp->id = htons(currpid);
	    id = icmp->id;
	    icmp->seq = htons(i);
	    seq = icmp->seq;
	    icmp->cksum = 0;
 	    icmp->cksum = checksum((uchar *)icmp, REQUEST_PKTSZ-ETHER_SIZE-IPv4_SIZE); //32 estimated, needs to be dynamic
	    //ippkt->data[1] = icmp;		    
	    //request->data[1] = ippkt;
	    //  Write the constructed packet to the ethernet device.
	    //kprintf("ping:");rawPrint(request, PKTSZ);//arpPrint(ether, PKTSZ);
	    totalsent++;
	    write(ETH0, request, REQUEST_PKTSZ);
	    sleep(1000); 
            //  Read from the ethernet device and sleep. 
	    
	    endLoop = 0;
	    while((!endLoop) && (attempts < MAX_READ_ATTEMPTS))
	    {  
	        while((length = read(ETH0, ether, REQUEST_PKTSZ)) == 0);
		attempts++;
		//  Reply to ARP requests using arp_reply() if appropriate.
		if (ether->type == htons(ETYPE_ARP))
	    	{
		    //kprintf("in:");rawPrint(ether, PKTSZ);//arpPrint(ether, PKTSZ);
		    arp = (struct arpgram *)ether->data;
	            ether_swap(ether);
		    if(arp->oper == ntohs(ARP_REQUEST))
		    { 
		    	if (SYSERR == arp_reply(arp))
		    	{
			    
			}
			else
			{
	    	        	write(ETH0, ether, length);
			}
		    //kprintf("out:");rawPrint(ether, PKTSZ);//arpPrint(request, PKTSZ);
		    //kprintf("xsh_ping.c (echoRequest) arpReply - reply packet sent\r\n");
		    }
	    	}
	        if(ether->type == htons(ETYPE_IPv4))
		{
	    		ippkt = (struct ipv4gram *)ether->data;	    
	    		if (ippkt->protocol == IP_ICMP)
			{
				icmp = (struct icmpgram *)ippkt->data;
				if (icmp->type == ntohs(ICMP_REPLY))
					endLoop = 1;
			}
		}
	    }
	    if((icmp->type == ICMP_REPLY))
	    	icmpPrint(ether->data, ntohs(ippkt->length));
    	    else
		dropped++;
    } 
  
    kprintf("Dropped %d packets of %d all requests sent.\r\n", dropped, totalsent);
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
