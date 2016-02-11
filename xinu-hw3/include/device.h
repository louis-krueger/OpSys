/**
 * @file device.h 
 * @provides isbaddev
 *
 * Contains all definitions relating to the XINU device subsystem.
 *
 * $Id: device.h 175 2007-07-12 01:18:27Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/* Device table declarations */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <kernel.h>

/* Device table entry */
typedef struct dentry
{
	ushort   num;                             /**< device number            */
	char    *name;                            /**< device name              */
	devcall (*init)   (struct dentry *, ...); /**< init function            */
	devcall (*open)   (struct dentry *, ...); /**< open function            */
	devcall (*close)  (struct dentry *, ...); /**< close function           */
	devcall (*read)   (struct dentry *, ...); /**< read function            */
	devcall (*write)  (struct dentry *, ...); /**< write function           */
	devcall (*seek)   (struct dentry *, ...); /**< seek function            */
	devcall (*getc)   (struct dentry *, ...); /**< getc function            */
	devcall (*putc)   (struct dentry *, ...); /**< putc function            */
	devcall (*control)(struct dentry *, ...); /**< control function         */
	void    *csr;                             /**< ctrl & status reg addr   */
	uchar   inmask;                           /**< input vector             */
	uchar   outmask;                          /**< output vector            */
	void    (*inintr)(void);                  /**< input interrupt handler  */
	void    (*outintr)(void);                 /**< output interrupt handler */
	void    *controlblk;                      /**< control block            */
	ushort  minor;                            /**< minor device number      */
} device;

extern struct dentry devtab[];                /* one entry per device     */

/* Device name definitions */
#define	SERIAL0  0                 /**< type uart                   */
#define	SERIAL1  1                 /**< type uart                   */
#define CONSOLE  2                 /**< type tty                    */
#define TTY0     2                 /**< type tty                    */
#define TTY1     3                 /**< type tty                    */

/* Control block sizes */
#define	NUART    2                 /**< number of UART              */
#define	NTTY     2                 /**< number of TTY               */
#define	NWIN     0                 /**< number of windowing devices */
#define NFRAMBUF 0                 /**< number of frame buffers     */

/* Number of devices */
#define	NDEVS ( NUART + NTTY + NFRAMBUF + NWIN )

/**
 * Check that device id is between 0 and NDEVS.  All calls to isbaddev will be
 * ushort, so no need to check f < 0.
 *
 * @param f id number to test
 */
#define isbaddev(f)  ( (ushort)(f)>=NDEVS )

/* Standard driver functions */
devcall close(ushort);
devcall control(ushort, ushort, long, long);
devcall getc(ushort);
devcall open(ushort, ...);
devcall putc(ushort, char);
devcall read(ushort, void *, long);
devcall write(ushort, void *, long);
devcall seek(ushort, long);

#endif /* _DEVICE_H_ */
