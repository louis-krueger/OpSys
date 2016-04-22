/* Device table declarations */
#include <kernel.h>

#ifndef _DEVICE_H_
#define _DEVICE_H_

typedef struct dentry
{                               /*   Device table entry                   */
    int dvnum;                  /**< Device number                        */
    char *dvname;               /**< Name of device                       */
    devcall (*dvinit) (struct dentry *, ...);  /**< Initialization function */
    devcall (*dvopen) (struct dentry *, ...);  /**< Open function         */
    devcall (*dvclose) (struct dentry *, ...); /**< Close function        */
    devcall (*dvread) (struct dentry *, ...);  /**< Read function         */
    devcall (*dvwrite) (struct dentry *, ...); /**< Write function        */
    devcall (*dvgetc) (struct dentry *, ...);  /**< Byte input function   */
    devcall (*dvputc) (struct dentry *, ...);  /**< Byte output function  */
    devcall (*dvseek) (struct dentry *, ...);  /**< Seek function         */
    devcall (*dvcntl) (struct dentry *, ...);  /**< Control function      */
    int dvcsr;                  /**< Device control and status registers  */
    int mask;                   /**< Interrupt mask                       */
    void (*intr) (void);        /**< Interrupt handler                    */
    char *dvioblk;              /**< Device-specific I/O control block    */
    int dvminor;                /**< Minor number                         */
} device;                       /*   Device table entry                   */

extern struct dentry devtab[];  /* one entry per device */

/**
 * \defgroup device Device driver API
 *
 * The Device Driver interface provides a unified set of functions
 *  for accessing Xinu devices.
 *
 * @{
 */
/* Device name definitions */
#define CONSOLE     0           /**< CONSOLE is on the first serial port. */
#define TTY0        0           /* type tty      */
#define TTY1        1           /**< TTY1 is the second serial port.      */
#define DISK0       2
#define ETH0        3           /**< ETH0 is the first Ethernet device.   */

/* Control block sizes */

#define	NTTY	 2
#define NDISK    1
#define NETHER 1

#define	NDEVS	(NTTY + NDISK + NETHER)

#define isbaddev(f)     ( (f)<0 || (f)>=NDEVS )

devcall close(int);
devcall control(int, int, int, int);
devcall getc(int);
devcall open(int, ...);
devcall putc(int, char);
devcall read(int, void *, int);
devcall seek(int, int);
devcall write(int, void *, int);
/**@}*/
#endif                          /* _DEVICE_H_ */
