/* file.h */

#ifndef __XINU_FILE_H__
#define __XINU_FILE_H__

#include <disk.h>
#include <device.h>
#include <semaphore.h>

#define FNAMLEN   7
#define DISKMAGIC 0x0B0BBCCD
#define FILEMAX   32

#define SUPERBLOCKNUM 0

/* This struct contains top-level metadata for a file. */
struct filenode
{
    int fn_state;               /* FILE_* below                 */
    int fn_length;              /* file length in bytes         */
    int fn_cursor;              /* cursor position in file      */
    int fn_blocknum;            /* disk block containing file   */
    char fn_name[FNAMLEN + 1];
    char *fn_data;              /* file data                    */
};

/* FILE states */

#define	FILE_FREE	0
#define FILE_USED       1
#define	FILE_OPEN	2
#define FILE_DIRTY      4

/* This global table contains an fnode for every open file.     */
extern struct filenode *filetab;

#define DIRENTRIES ((DISKBLOCKLEN - sizeof(int)) / sizeof(struct filenode))
struct dirblock
{
    int db_blocknum;
    struct filenode db_fnodes[DIRENTRIES];
};

#define isbadfd(n) (((n) < 0) || ((n) >= DIRENTRIES))

/* Linked list keeps track of free disk blocks.          */
#define FREEBLOCKMAX ((DISKBLOCKLEN / sizeof(int)) - 3)
struct freeblock
{
    int fr_blocknum;
    int fr_count;
    struct freeblock *fr_next;
    int fr_free[FREEBLOCKMAX];
};

/* Superblock contains information about filesystem.             */
#define MAXFILES ((DISKBLOCKLEN / sizeof(int)) - 2)
struct superblock
{
    int sb_magic;
    int sb_blocknum;            /* Block containing superblock */
    int sb_blocktotal;          /* Total blocks in file system */
    struct freeblock *sb_freelst;
    semaphore sb_freelock;
    struct dirblock *sb_dirlst;
    semaphore sb_dirlock;
    struct dentry *sb_disk;
};

/* Global table contains a superblock node for every open file system. */
extern struct superblock *supertab;

devcall fileOpen(char *);
devcall fileClose(int);
devcall fileCreate(char *);
devcall fileDelete(int);
devcall fileGetChar(int);
devcall filePutChar(int, char);
devcall fileSeek(int, int);

devcall sbInit(struct superblock *, int);
devcall sbOpen(struct disk *);
devcall sbFreeBlock(struct superblock *, int);
devcall sbGetBlock(struct superblock *);

#endif
