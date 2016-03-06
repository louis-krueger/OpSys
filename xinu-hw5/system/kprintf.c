/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */

static unsigned char ungetArray[UNGETMAX];
static int ungetIndex = 0;


/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    uchar c;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x20201000;

    // TODO: First, check the unget buffer for a character.
    //       Otherwise, check UART line status, and
    //       when the receiver has something, get character c.
    if (ungetIndex > 0)
    {
        c = ungetArray[--ungetIndex];
        if (0x04 == c)
        {
            ungetArray[ungetIndex++] = 0x04;
            return EOF;
        }
        return c;
    }

    /* Wait until a character is ready to be received.  */
    while ((regptr->fr & PL011_FR_RXFE))
    {
        /* Do nothing */
    }

    /* Get the next character from the UART by reading it from the Data
     * Register.  */
    c = regptr->dr;

    if (0x04 == c)
    {
        ungetArray[ungetIndex++] = 0x04;
        return EOF;
    }

    return c;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x20201000;
    return (ungetIndex || (!(regptr->fr & PL011_FR_RXFE)));
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    if (ungetIndex < UNGETMAX)
    {
        ungetArray[ungetIndex++] = c;
        return c;
    }
    return SYSERR;
}


/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x20201000;

    /* Wait until UART is ready for another character  */
    while ((regptr->fr & PL011_FR_TXFF))
    {
        /* Do nothing */
    }

    /* Put the character to the UART by writing it to UART's data register. */
    regptr->dr = c;

    return c;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    preempt = 0;
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    return retval;
    preempt = QUANTUM;
}
