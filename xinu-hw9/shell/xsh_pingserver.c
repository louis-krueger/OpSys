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
    int length;

    ether = (struct ethergram *)packet;

    enable();

    while (TRUE)
    {
        bzero(ether, PKTSZ);

        while ((length = read(ETH0, (void *)ether, PKTSZ)) == 0)
            ;

        // TODO: Decode and respond to ARP Request and ICMP Echo Requests.
        //  Replies can be sent with
        //  write(ETH0, (void *)packet, length);
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
