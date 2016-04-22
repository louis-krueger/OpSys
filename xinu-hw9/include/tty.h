/* tty.h */
#include <semaphore.h>
#include <uart.h>

#ifndef _TTY_H_
#define _TTY_H_

#define	IBLEN		1024    /* input buffer size                    */
#define	OBLEN		1024    /* output buffer size               */


struct tty
{
    unsigned char state;        /* TTYS_* below                     */
    struct dentry *pdev;        /* tty dev structure                */
    struct uart_csreg *pucsr;   /* UART control/status regs         */
    /* TTY input fields */
    semaphore isema;            /* input available semaphore        */
    unsigned char iflags;       /* TIF_* below                      */
    unsigned short istart;      /* index of first character         */
    unsigned short icount;      /* # characters in input buffer     */
    unsigned char in[IBLEN];

    /* TTY output fields */
    semaphore osema;            /* output buffer space semaphore */
    unsigned char oflags;       /* TOF_* below                      */
    unsigned short ostart;      /* index of first character     */
    unsigned short ocount;      /* # characters in output buffer */
    unsigned char out[OBLEN];
    int oidle;                  /* TTY transmitter idle         */
};

/* TTY states */

#define	TTYS_FREE	0
#define	TTYS_ALLOC	1

/* TTY input flags */

/* TTY output flags */

/* control() functions */

#define	TTC_GIF		0x0001  /* get input flags              */
#define	TTC_GOF		0x0002  /* get output flags             */

#define TTC_SIF     0x0003      /* set input flags              */

/* TTY input flags */

#define TIF_NOBLOCK  0x0001     /* don't block on input         */
#define TIF_NOECHO   0x0002     /* don't echo input             */

extern struct tty ttytab[];

devcall ttyClose(struct dentry *);
devcall ttyControl(struct dentry *, int, char *, char *);
int ttyGetChar(struct dentry *);
devcall ttyInit(struct dentry *);
devcall ttyOpen(struct dentry *);
int ttyPutChar(struct dentry *, uchar);
int ttyRead(struct dentry *, char *, int);
int ttyWrite(struct dentry *, uchar *, int);
void ttyIntr(void);

#endif                          /* _TTY_H_ */
