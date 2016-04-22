/**
 * @file platform.h
 * @provides 
 *
 * $Id: platform.h 184 2007-07-12 22:52:09Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

struct platform
{
    char name[16];
    void *maxaddr;
    ulong time_base_freq;
    uchar uart_dll;
    uchar uart_irqnum;
};

extern struct platform platform;

/* Max RAM addresses */
#define MAXADDR_DEFAULT     0x00800000  /**< default  8MB RAM            */
#define MAXADDR_WRT54G      0x00800000  /**< G    has 8MB RAM            */
#define MAXADDR_WRT54GL     0x01000000  /**< GL   has 16MB RAM           */
#define MAXADDR_WRT350N     0x02000000  /**< 350N has 32MB RAM           */

/* Time Base Frequency */
#define TIME_BASE_FREQ_WRT54G   120000000
#define TIME_BASE_FREQ_WRT54GL  100000000
#define TIME_BASE_FREQ_WRT350N  150000000
#define TIME_BASE_FREQ_DEFAULT  100000000

/* UART DLL (Divisor Latch LS) */
#define UART_DLL_WRT54G     0x0E
#define UART_DLL_WRT54GL    0x0B
#define UART_DLL_WRT350N    0x29
/* Baud divisor of 0x000B (11d) seems to work for the GL UART.        */
/* Assuming x16 clock factor, that gives a base crystal frequency of  */
/* about 20.275 MHz.                                                  */
