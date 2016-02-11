/**
 * @file start.S
 * @provides _start, copyhandler, clearvector, getmaxaddr
 *
 * $Id: start.S 184 2007-07-12 22:52:09Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * This is where the common firmware environment begins to execute code.
 */

#include <mips.h>
#include <interrupt.h>

#define CLOCK_BASE_FREQ  100000000
			
.text
	.align	4
	.globl _start
	
/**
 * The Common Firmware Environment will load the image into temporary memory 
 * via TFTP, and will then jump to this (_start) label at address 0x80001000
 * The boot loader (_start) copies the kernel's interrupt handler to the
 * proper location and clears the space for the XINU-defined trap and 
 * interrupt vecotrs.  It also flushes the L1 instruction and data caches.  
 * The base address of UART0, the maximum physical address, and the clock
 * base frequency are passed to the kernel (_startup).
 * @param a0 firmware handle
 * @param a1 zero
 * @param a2 firmware entry vector
 * @param a3 entry point seal
 */
_start:
	/* Copy IRQ transfer handler to reserved memory location */
	la    a0, transferhandler  /* Starting address              */
	la    a1, endtranshandler  /* Ending address                */
	la    a2, IRQ_ADDR         /* Destination address           */
	jal   copyhandler
	nop

	/* Clear XINU-defined trap and interrupt vectors */
	la    a0, TRAPVEC_ADDR
	la    a1, IRQVEC_END
	jal   clearvector
	
	/* Clear interrupt related registers in the coprocessor */	
	mtc0  zero, CP0_STATUS     /* Clear interrupt masks           */
	mtc0  zero, CP0_CAUSE      /* Clear interrupt cause register  */

	/* Clear and invalidate the L1 instruction and data caches */
	jal   flushcache
	nop
	
	/* Pass control to XINU kernel.	 Send hard-coded parameters   */
	/*  that a more elaborate boot loader would have discovered.  */
	jal   getcpuid             /* Load Processor ID               */           
	move  a0, v0               /* Processor ID                    */
	j     _startup
	nop

/**
 * Copy text (code) from source to detination.
 * copyhandler(int *srcBegin, int *srcEnd, int *dstBegin)
 * @param srcBegin start of the source code
 * @param srcEnd end of the source code
 * @param dstBegin start of the destination
 */
copyhandler:
	lw    a3, 0(a0)
	sw    a3, 0(a2)
	addu  a0, 4
	addu  a2, 4
	blt   a0, a1, copyhandler
	jr    ra
	nop

/**
 * Zero memory from dstBegin to dstEnd (non-inclusive).	
 * clearvector(int *dstBegin, int *dstEnd)
 * @param dstBegin start of the memory area of zero
 * @param dstEnd end of the memory area to zero
 */
clearvector:
	sw    zero, 0(a0)
	addu  a0, 4
	blt   a0, a1, clearvector
	jr    ra
	nop

/**
 * Get the processor ID. 
 * @return current value from the PRID register.
 */
getcpuid:
	mfc0  v0, CP0_PRID         /* obtain processor id register    */
	jr    ra
	nop
