/**
 * @file network.h
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

/* Little Endian converters */
#define htons(x) ((((x)>>8) &0xff) | (((x) & 0xff)<<8))
#define ntohs(x) htons(x)
#define htonl(x) ((((x)& 0xff)<<24) | (((x)>>24) & 0xff) | \
		                   (((x) & 0xff0000)>>8) | (((x) & 0xff00)<<8))
#define ntohl(x) htonl(x)


/* Networking Constants */
#define ETH_ADDR_LEN 6          /* MAC address length       */
#define IP_ADDR_LEN  4          /* IP address length        */
#define PKTSZ        ETH_HEADER_LEN + ETH_MTU   /* Maximum packet size      */

/* Ethernet packet types */
#define ETYPE_IPv4 0x0800
#define ETYPE_ARP  0x0806
#define ETYPE_FISH 0x3250

/* Ethergram header size    */
#define ETHER_SIZE   (ETH_ADDR_LEN * 2 + 2)

/* Minimum payload size */
#define ETHER_MINPAYLOAD 46

/*
 * Ethernet HEADER
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Destination MAC Address                                       
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   ...                           | Source MAC Address             
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Frame Payload Type            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct ethergram             /**< Ethernet Packet Variables             */
{
    uchar dst[ETH_ADDR_LEN];    /**< Ethernet destination MAC Address   */
    uchar src[ETH_ADDR_LEN];    /**< Ethernet source MAC Address        */
    ushort type;             /**< Ethernet Packet Type                  */
    char data[1];            /**< Dummy field that refers to payload    */
};

/* IP Packet Definitions */
#define IP_V4       4
#define IP_TTL      64
#define IP_VER      0xF0
#define IP_IHL      0x0F
#define IP_FLAG_DF  0x2

/* IP Types */
#define IP_ICMP     0x01
#define IP_UDP      0x11
#define IP_TCP      0x06

/* IP Header Length */
#define IPv4_SIZE   (12 + IP_ADDR_LEN * 2)

/* IP Packet Structure */
struct ipv4gram
{
    uchar  ver_hlen;
    uchar  tos;
    ushort length;
    ushort id;
    ushort froff;
    uchar  ttl;
    uchar  protocol;
    ushort cksum;
    uchar  src[IP_ADDR_LEN];
    uchar  dst[IP_ADDR_LEN];
    char   data[1];
};


/* ICMP Packet Definitions */
#define ICMP_REQUEST 0x08       /* Ping Request     */
#define ICMP_REPLY   0x00       /* Ping Response    */

/* ICMP Packet Structure */
struct icmpgram
{
    uchar type;
    uchar code;
    ushort cksum;
    ushort id;
    ushort seq;
    uchar data[1];
};

/* ARP Packet Definitions */
#define ARP_REQUEST 0x0001
#define ARP_REPLY   0x0002

/* ARP Header Size */
#define ARP_SIZE (8 + ETH_ADDR_LEN * 2 + ENET_IPv4 * 2)

/* ARP Packet Structure */
struct arpgram
{
    ushort htype;
    ushort ptype;
    uchar hlen;
    uchar plen;
    ushort oper;
    uchar sha[ETH_ADDR_LEN];
    uchar spa[IP_ADDR_LEN];
    uchar tha[ETH_ADDR_LEN];
    uchar tpa[IP_ADDR_LEN];
};


/* Ping Server Constants */
#define MAX_REQUESTS        20
#define MAX_READ_ATTEMPTS   10
#define REQUEST_PKTSZ       96
#define ARP_PKTSZ           42
#define PING_GATEWAY      "52:54:00:5A:4D:06\0"     /* zardoz */

/* Networking functions */
int arp_reply(void *buf);
int icmp_reply(void *buf, int length);
int icmp_request(void *buf, ushort id, ushort seq, char * dst);
int icmp_reqreply(void *buf, int length);
int replyserver(void);
int requestserver(char *dst);

