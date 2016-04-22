/* disk.h */

#ifndef __XINU_DISK_H__
#define __XINU_DISK_H__

#include <device.h>
#include <stdarg.h>
#include <semaphore.h>

#define DISKBLOCKTOTAL  256     /* disk blocks                  */
#define	DISKBLOCKLEN	256     /* disk block size              */
#define DOPEN_RETRY     10      /* retries to open disk connect */

struct disk
{
    unsigned char disk_state;   /* DISKS_* below                */
    struct dentry *disk_pdev;   /* disk's dev structure         */
    struct dentry *disk_phw;    /* hardware dev structure       */

    int disk_size;              /* number of disk blocks        */
    int disk_current;           /* current block position       */
    semaphore disk_lock;        /* Mutex for disk access        */
};

/* DSK states */

#define	DISKS_FREE	0
#define	DISKS_ALLOC	1

extern struct disk disktab[];

devcall diskInit(struct dentry *);
devcall diskOpen(struct dentry *, va_list);
devcall diskRead(struct dentry *, char *, int);
devcall diskWrite(struct dentry *, char *, int);
devcall diskSeek(struct dentry *, long);
#endif
