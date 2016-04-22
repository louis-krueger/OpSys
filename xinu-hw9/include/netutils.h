/**
 * @file netutils.h
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

/* Network helper functions */
int dot2ip(char *, uchar *);
ushort checksum(void *addr, int length);
int getmac(void *buf);
int getip(void *buf);

int ether_swap(void *buf);
int ipv4_swap(void *buf, int length);

