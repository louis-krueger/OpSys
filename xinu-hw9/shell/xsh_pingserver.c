/**
 * @file     xsh_pingserver.c
 * @provides xsh_pingserver, echoReply
 *
 */
/* Embedded XINU, Copyright (C) 2009, 2016.  All rights reserved. */

#include <xinu.h>

/**
 * Process replies to incoming echo requests.
 */
process echoReply(void)
{
    char packet[PKTSZ];
    struct ethergram *ether;
    struct ipv4gram *ipgram;
    struct icmpgram *icmp;
    struct arpgram *arp;
    int length;

    ether = (struct ethergram *)packet;

    enable();

    while (TRUE)
    {
        bzero(ether, PKTSZ);
        while ((length = read(ETH0, (void *)ether, PKTSZ)) == 0);

        // TODO: Decode and respond to ARP Request and ICMP Echo Requests.
        //  Replies can be sent with
        //  write(ETH0, (void *)packet, length);
        if (ether->type == htons(ETYPE_ARP))
	{
		kprintf("arpreply entered\r\n");
		arp = (struct arpgram *)ether->data;
		if (SYSERR == arp_reply(arp))
		{
			kprintf("syserr\r\n");
		}
		else
		{
			ether_swap(ether);
			ether->type = htons(ETYPE_ARP);
			ether->data[1] = arp;
			write(ETH0, ether, ARP_PKTSZ);
		}
	}
	else if (ether->type == htons(ETYPE_IPv4))
	{
		kprintf("ipv4reply entered\r\n");
		ipgram = (struct ipv4gram *)ether->data;
		icmp = (struct icmpgram *)ipgram->data;
		icmpPrep(ether, currpid, ether->dst);
		ether_swap(ether);
		ether->type = htons(ETYPE_IPv4);
		icmp->type = htons(ICMP_REPLY);
		write(ETH0, ether, PKTSZ);
	}
    }

    return 0;
}



/**
 * Shell command (pingserver) provides a simple ping server.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_pingserver(ushort nargs, char *args[])
{
    ready(create((void *)echoReply, INITSTK, 19, "EchoReply", 0),
          RESCHED_YES);
    return 0;
}
