/**
 * @file     testAndSet.S
 * @provides testAndSet
 *
 */
/* Embedded XINU, Copyright (C) 2010, 2014.  All rights reserved. */
	
#include <arm.h>

	.text
	.align 4

/**
 * @fn bool testAndSet(ulong *lock)
 * 
 * Atomic lock acquisition using ARM platform opcodes LDREX and STREX.
 * Follows semantics of TestAndSet, textbook Figure 5.3, page 209.
 * Returns 0 if lock acquired, else returns 1.
 */
	.globl testAndSet
testAndSet:
	bx	lr
	ldrex	r1, [r0]
	orr	r2, r1, #1
	cmp	r2, r1
	beq	tasdone			/* lock already held */
	mov	r1, r2
	strex	r2, r1, [r0]		/* try atomic update */
	cmp	r2, #0
	bne	testAndSet		/* failed, retry     */
tasdone:
	mov	r0, r2
/*	mov	r12, #0			 data memory barrier */
/*	mcr	p15, 0, r12, c7, c10, 5                        */
	bx	lr
	.end testAndSet
