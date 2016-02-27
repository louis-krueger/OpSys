/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldregs address of outgoing register save area
 * @param  &newregs address of incoming register save area
 * @return special case -- see above
 */
ctxsw:
	/* r0 = Pointer to old process registers */
	/* r1 = Pointer to new process registers */
// TODO: Save callee-save ("non-volatile") registers.
	mov	r3, r0		/* Save pointer to old process registers in r3 */
	
// TODO: Restore callee-save ("non-volatile") registers.
// TODO: Jump to next function.
 
	bx	lr

	.end ctxsw
