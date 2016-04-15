/**
 * @file ether.h
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#ifndef _ETHER_H_
#define _ETHER_H_

#include <device.h>
#include <stdarg.h>
#include <kernel.h>
#include <semaphore.h>
#include <bcm4713.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

#define ETH_ADDR_LEN 6          /* MAC address length       */
#include <vlan.h>

/* ETH Buffer lengths */
#define ETH_IBLEN           1024 /**< input buffer size                 */
#define ETH_RX_RING_ENTRIES 511 /**< Number of buffers on Rx Ring       */
#define ETH_TX_RING_ENTRIES 511 /**< Number of buffers on Tx Ring       */

/* Ethernet memory regions */
#define ETH_RING_PAGES      1   /**< Ring buffer size in pages          */
#define ETH_DMA_TABLE_PAGES 1   /**< DMA Table size in pages            */

/* Ethernet DMA buffer sizes */
#define ETH_MTU             1500 /**< Maximum transmission units        */
#define ETH_HEADER_LEN      14  /**< Length of Ethernet header          */
#define ETH_VLAN_LEN        4   /**< Length of Ethernet vlan tag        */
#define ETH_CRC_LEN         4   /**< Length of Ethernet CRC             */
#define ETH_MAX_PKT_LEN     ( ETH_HEADER_LEN + ETH_VLAN_LEN + ETH_MTU )

#define ETH_RX_BUF_SIZE     ( ETH_MAX_PKT_LEN + ETH_CRC_LEN \
                              + sizeof(struct rxHeader) )
#define ETH_TX_BUF_SIZE     ( ETH_MAX_PKT_LEN )

/* ETH states */
#define ETH_STATE_FREE       0
#define ETH_STATE_DOWN       1
#define ETH_STATE_UP         2

/* ETH control codes */
#define ETH_CTRL_CLEAR_STATS 1  /**< Clear Ethernet Statistics          */
#define ETH_CTRL_SET_MAC     2  /**< Set the MAC for this device        */
#define ETH_CTRL_GET_MAC     3  /**< Get the MAC for this device        */
#define ETH_CTRL_SET_RX_MODE 4  /**< Set Rx Mode                        */
#define ETH_CTRL_RESET       5  /**< Reset the Ethernet device          */
#define ETH_CTRL_DISABLE     6  /**< Disable the Ethernet device        */

/* Reciever Header struct and constants */
#define ETH_RX_FLAG_OFIFO   0x0001 /**< FIFO Overflow                   */
#define ETH_RX_FLAG_CRCERR  0x0002 /**< CRC Error                       */
#define ETH_RX_FLAG_SERR    0x0004 /**< Receive Symbol Error            */
#define ETH_RX_FLAG_ODD     0x0008 /**< Frame has odd number nibbles    */
#define ETH_RX_FLAG_LARGE   0x0010 /**< Frame is > RX MAX Length        */
#define ETH_RX_FLAG_MCAST   0x0020 /**< Dest is Multicast Address       */
#define ETH_RX_FLAG_BCAST   0x0040 /**< Dest is Broadcast Address       */
#define ETH_RX_FLAG_MISS    0x0080 /**< Received due to promisc mode    */
#define ETH_RX_FLAG_LAST    0x0800 /**< Last buffer in frame            */
#define ETH_RX_FLAG_ERRORS  ( ETH_RX_FLAG_ODD | ETH_RX_FLAG_SERR | \
                              ETH_RX_FLAG_CRCERR | ETH_RX_FLAG_OFIFO )

/**
 * Header on a received packet.
 */
struct rxHeader
{
    ushort length;              /**< Length of packet data              */
    ushort flags;               /**< Receive flags                      */
    ushort pad[12];             /**< Padding                            */
};

/**
 * Ethernet packet buffer
 */
struct ethPktBuffer
{
    uchar *buf;                 /**< Pointer to buffer space            */
    uchar *data;                /**< Start of data within buffer        */
    int length;                 /**< Length of packet data              */
};

/* Ethernet DMA descriptor */
#define ETH_DESC_CTRL_LEN   0x00001fff /**< Mask for length field       */
#define ETH_DESC_CTRL_CMASK 0x0ff00000 /**< Core specific bits          */
#define ETH_DESC_CTRL_EOT   0x10000000 /**< End of Table                */
#define ETH_DESC_CTRL_IOC   0x20000000 /**< Interrupt On Completion     */
#define ETH_DESC_CTRL_EOF   0x40000000 /**< End of Frame                */
#define ETH_DESC_CTRL_SOF   0x80000000 /**< Start of Frame              */

/**
 * Descriptor for the DMA engine to determine where to find a packet
 * buffer.
 */
struct dmaDescriptor
{
    ulong control;              /**< DMA control bits                   */
    ulong address;              /**< Stored as physical address         */
};

/* Ethernet control block */
#define ETH_INVALID  (-1)       /**< Invalid data (virtual devices)     */

/**
 * Ethernet control block
 */
struct ether
{
    uchar state;                /**< ETH_STATE_*above                   */
    device *phy;                /**< physical eth device for Tx DMA     */

    /* Pointers to associated structures */
    device *dev;                /**< eth device structure               */
    struct bcm4713 *csr;        /**< control and status registers       */

    ulong interruptMask;        /**< interrupt mask                     */
    ulong interruptStatus;      /**< interrupt status                   */

    struct dmaDescriptor *rxRing; /**< array receive ring descs.*/
    struct ethPktBuffer **rxBufs; /**< Rx ring array          */
    ulong rxHead;               /**< Rx ring head index                 */
    ulong rxTail;               /**< Rx ring tail index                 */
    ulong rxPending;            /**< Number of Rx ring descriptors      */
    ulong rxirq;                /**< Count of Rx interrupt requests     */
    ulong rxOffset;             /**< Size in bytes of rxHeader          */
    ulong rxErrors;             /**< Count of Rx errors.                */

    struct dmaDescriptor *txRing; /**< array transmit ring descs.*/
    struct ethPktBuffer **txBufs; /**< Tx ring array          */
    ulong txHead;               /**< Tx ring head index                 */
    ulong txTail;               /**< Tx ring tail index                 */
    ulong txPending;            /**< Number of Tx ring descriptors      */
    ulong txirq;                /**< Count of Tx interrupt requests     */

    uchar devAddress[ETH_ADDR_LEN];

    uchar addressLength;        /**< Hardware address length            */
    ushort mtu;                 /**< Maximum transmission units         */

    ulong errors;               /**< Number of Ethernet errors          */
    ushort ovrrun;              /**< Buffer overruns                    */
    semaphore isema;            /**< I/0 sem for eth input              */
    ushort istart;              /**< Index of first byte                */
    ushort icount;              /**< Packets in buffer                  */

    struct ethPktBuffer *in[ETH_IBLEN]; /**< Input buffer               */

    int inPool;                 /**< buffer pool id for input           */
    int outPool;                /**< buffer pool id for output          */
};

extern struct ether ethertab[];

/* Driver functions */
devcall etherInit(device *);
devcall etherOpen(device *);
devcall etherClose(device *);
devcall etherRead(device *, void *, ulong);
devcall etherWrite(device *, void *, ulong);
devcall etherControl(device *, int, long, long);
interrupt etherInterrupt(void);

int colon2mac(char *, uchar *);
int allocRxBuffer(struct ether *, int);
int waitOnBit(volatile ulong *, ulong, const int, int);

/* Network helper functions */
int dot2ip(char *, uchar *);
ushort checksum(void *addr, int length);

#endif                          /* _ETHER_H_ */
