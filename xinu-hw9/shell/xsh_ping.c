/**
 * @file     xsh_ping.c
 * @provides xsh_ping, echoRequest
 *
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
    int length;
    int attempts, totalsent, dropped;
    ushort id, seq;
    bool endLoop;

    totalsent = 0;
    dropped = 0;
    id = 0;
    seq = 0;
    request = (struct ethergram *)requestpkt;
    ether = (struct ethergram *)receivepkt;

    // TODO: Zero out memory for receiving packets.
    //  Construct an ICMP echo request packet.  (See icmpPrep() for help)
    //  Write the constructed packet to the ethernet device.
    //  Read from the ethernet device and sleep.
    //  Reply to ARP requests using arp_reply() if appropriate.
    //  Print reply packets (icmpPrint()) and keep stats.

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
