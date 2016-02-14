/**
 * @file kprintf.c
 * @Authors: Louis Krueger & Samuel Scheel
 * TA-BOT:MAILTO louis.krueger@marquette.edu samuel.scheel@marquette.edu
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */


/*	struct pl011_uart info from header
 *	
 *	struct pl011_uart_csreg
 *	{
 *		volatile unsigned int dr; 	    **< Data Register                        *///0x00
/*		volatile unsigned int rsrecr;       **< Receive status/error clear register  *///0x04
/*		volatile unsigned int dont_use_a;   ** spacer                                *///0x08
/*		volatile unsigned int dont_use_b;   ** spacer                                *///0x0C	
/*		volatile unsigned int dont_use_c;   ** spacer                                *///0x10
/*		volatile unsigned int dont_use_d;   ** spacer                                *///0x14
/*		volatile unsigned int fr;   	    **< Flag Register                        *///0x18
/*		volatile unsigned int dont_use_e;   ** spacer                                *///0x1C
/*		volatile unsigned int ilpr;         **< Not in use                           *///0x20
/*		volatile unsigned int ibrd; 	    **< Integer baud rate divisor            *///0x24
/*		volatile unsigned int fbrd; 	    **< Fractional baud rate divisor         *///0x28
/*		volatile unsigned int lcrh; 	    **< Line Control Register                *///0x2C
/*		volatile unsigned int cr;   	    **< Control Register                     *///0x30
/*		volatile unsigned int ifls; 	    **< Interupt FIFO level select register  *///0x34
/*    		volatile unsigned int imsc; 	    **< Interupt Mask Set Clear Register     *///0x38
/*   		volatile unsigned int ris;  	    **< Raw Interupt Status Register         *///0x3C
/*    		volatile unsigned int mis;  	    **< Masked Interupt Status Register      *///0x40
/*    		volatile unsigned int icr;  	   **< Interupt Clear Register              *///0x44

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */

static unsigned char ungetArray[UNGETMAX];

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
	uchar c;
	volatile struct pl011_uart_csreg *regptr = (struct pl011_uart_csreg *)0x20201000;    /*points to pl011 uart struct*/
	sint uartflag = (*regptr).fr;   /*store uart flag register in sint 'uartflag' */
	

	// TODO: First, check the unget buffer for a character.
	//       Otherwise, check UART flags register, and
	//       once the receiver is not empty, get character c. 
	int i = 0;
	for (i=0; i < UNGETMAX; i++)	
	{
		if (ungetArray[i] != '\0')
		{
			c = ungetArray[i];
			ungetArray[i] = '\0';
			return (int) c;
		}
	}
	//	     If unget buffer has char
	//	     else check UART flag register
	//	     get char from reciever
	while (1) //Poll I/O until receiver is not empty
	{
		//If UART flag registers tell us the receiver is not empty, receive character
		if ()
		{
			c = *regptr;
			return (int) c;
		}
    	}
	//       once the receiver is not empty, get character c. 
	return SYSERR;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
	volatile struct pl011_uart_csreg *regptr;
	regptr = (struct pl011_uart_csreg *)0x20201000;
	// TODO: Check the unget buffer and the UART for characters.
	int i = 0;
	for (i; i  < UNGETMAX; i++)
	{
		if (ungetArray[i] != '\0')
		//if we find a character, return true = 1
			return 1;
	}	
	//TODO: check for lingering characters in UART
	if (*regptr != (*regptr & 0xF00))
		return 1;
	else
	//if no characters are available return false = 0
		return 0;
	//return syscall integer
	return SYSERR;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
	// TODO: Check for room in unget buffer, put the character in or discard.
	int i = 0;
 	for (i; i < UNGETMAX; i++)
	{
		if (ungetArray[i] == '\0')
		//if unget buffer is not full, put character in the unget buffer
		{
			ungetArray[i] = c;
			return c;
		}
    	}
	//If buffer is full, discard c and return SYSERR
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
	signed int uartflag = regptr + UART_RF_OFFSET;   
	// TODO: Check UART flags register.
	//       Once the Transmitter FIFO is not full, send character c.
	while (1)  //Poll I/O until flags tell us FIFO is not full
	{
		if (*uartflag != (*uartflag & 0x20))
		//If uartflag tells us the tramit FIFO is not full, send character x
		{
			*regptr = c;
			return (int) c;
		}		
    	}
    	return SYSERR;
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
	int retval;
	va_list ap;

	va_start(ap, format);
	retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
	va_end(ap);
	return retval;
}
