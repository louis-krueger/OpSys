/* conf.h (GENERATED FILE; DO NOT EDIT) */

#ifndef _CONF_H_
#define _CONF_H_

#include <stddef.h>

/* Device table declarations */

/* Device table entry */
typedef struct dentry
{
    int num;
    int minor;
    char *name;
    devcall (*init) (struct dentry *);
    devcall (*open) (struct dentry *, ...);
    devcall (*close) (struct dentry *);
    devcall (*read) (struct dentry *, void *, uint);
    devcall (*write) (struct dentry *, const void *, uint);
    devcall (*seek) (struct dentry *, long);
    devcall (*getc) (struct dentry *);
    devcall (*putc) (struct dentry *, char);
    devcall (*control) (struct dentry *, int, long, long);
    void *csr;
    void (*intr) (void);
    uchar irq;
} device;

extern const device devtab[];   /* one entry per device */

/* Device name definitions */

#define SERIAL0     0           /* type uart     */
#define DEVNULL     1           /* type null     */
#define CONSOLE     2           /* type tty      */

/* Control block sizes */

#define NNULL 1
#define NUART 1
#define NTTY 1

#define DEVMAXNAME 20
#define NDEVS 3


/* Configuration and Size Constants */

#define LITTLE_ENDIAN 0x1234
#define BIG_ENDIAN    0x4321

#define BYTE_ORDER    LITTLE_ENDIAN

#define NTHREAD   100           /* number of user threads           */
#define NSEM      100           /* number of semaphores             */
/* #define NMAILBOX  15            number of mailboxes              */
#define RTCLOCK   TRUE          /* timer support                    */
#define NETEMU    FALSE         /* Network Emulator support         */
#define NVRAM     FALSE         /* nvram support                    */
#define SB_BUS    FALSE         /* Silicon Backplane support        */
#define USE_TLB   FALSE         /* make use of TLB                  */
#define USE_TAR   FALSE         /* enable data archives             */
#define NPOOL     8             /* number of buffer pools available */
#define POOL_MAX_BUFSIZE 2048   /* max size of a buffer in a pool   */
#define POOL_MIN_BUFSIZE 8      /* min size of a buffer in a pool   */
#define POOL_MAX_NBUFS   8192   /* max number of buffers in a pool  */
/* #define WITH_USB                USB support                      */
/* #define WITH_DHCPC              DHCP client support              */

#endif                          /* _CONF_H_ */
