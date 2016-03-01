/*
 * Written by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 * Embedded XINU, Copyright (C) 2013.  All rights reserved. */

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
	str	r0,  [r0,  #PREG_R0*4]
	str	r1,  [r0,  #PREG_R1*4]
	str	r2,  [r0,  #PREG_R2*4]
	str	r3,  [r0,  #PREG_R3*4]
	str	r4,  [r0,  #PREG_R4*4]
	str	r5,  [r0,  #PREG_R5*4]
	str	r6,  [r0,  #PREG_R6*4]
	str	r7,  [r0,  #PREG_R7*4]
	str	r8,  [r0,  #PREG_R8*4]
	str	r9,  [r0,  #PREG_R9*4]
	str	r10, [r0,  #PREG_R10*4]
	str	r11, [r0,  #PREG_R11*4]
	str	r12, [r0,  #PREG_IP*4]
	str	sp,  [r0,  #PREG_SP*4]
	str	lr,  [r0,  #PREG_LR*4]
	str	lr,  [r0,  #PREG_PC*4]
// TODO: Restore callee-save ("non-volatile") registers.
	ldr	r0,  [r1,  #PREG_R0*4]
	ldr	r2,  [r1,  #PREG_R2*4]
	ldr	r3,  [r1,  #PREG_R3*4]
	ldr	r4,  [r1,  #PREG_R4*4]
	ldr	r5,  [r1,  #PREG_R5*4]
	ldr	r6,  [r1,  #PREG_R6*4]
	ldr	r7,  [r1,  #PREG_R7*4]
	ldr	r8,  [r1,  #PREG_R8*4]
	ldr	r9,  [r1,  #PREG_R9*4]
	ldr	r10, [r1,  #PREG_R10*4]
	ldr	r11, [r1,  #PREG_R11*4]
	ldr	r12, [r1,  #PREG_IP*4]
	ldr	sp,  [r1,  #PREG_SP*4]
	ldr	lr,  [r1,  #PREG_LR*4]
// TODO: Jump to next function.
	mov     r12, r1
	ldr	r1,  [r1,  #PREG_R1*4]
	ldr	pc,  [r12, #PREG_PC*4]

	.end ctxsw
