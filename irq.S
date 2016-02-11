/**
 * @file     irq.S
 * @provides transferhandler, irqhandler, savestate
 *
 * $Id: irq.S 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */
	
#include <mips.h>
#include <interrupt.h>

	.text
	.align 4

/**
 * @fn void transferhandler(void)
 * 
 * Entry point for all XINU interrupts.
 * The transferhandler is copied to the memory address reserved for
 * interrupt/exception vectors by the loader.  The transferhandler
 * immediately transfers control to the proper handler, which is
 * too large to fit in the reserved area.
 * 
 * @see irqhandler()
 */
	.globl transferhandler
	.ent transferhandler
transferhandler:
	lui		k1, %hi(irqhandler)
	addiu	k0, k1, %lo(irqhandler)
	j		k0
	nop
	.globl endtranshandler
endtranshandler:
	.end transferhandler

		
/**
 * @fn void irqhandler(void)
 *
 * Main interrupt request handler.  Does quick decode of cause register to
 * allow fast response to critical handlers, otherwise falls through to
 * savestate which does full context save.  Note that because the state of all
 * registers must be preserved, this function must restrict itself to only use
 * the two kernel registers, k0 and k1.
 *
 * @see xtrap()
 */
	.globl	irqhandler
	.ent	irqhandler
irqhandler:
	.set noreorder
	/* Lookup cause of interrupt or exception. */
	mfc0	k0, CP0_CAUSE
	/* Check mask for exception bits.          */
	addiu	k1, zero, CAUSE_EXC
	and		k0, k0, k1
	/* If not an exception, go to generic      */
	/*  interrupt handler instead.             */
	beq		k0, zero, savestate
	/* Else use exception number as index into */
	/*  XINU-defined trap vector.              */
	lui		k1, %hi(TRAPVEC_ADDR)
	addiu	k1, %lo(TRAPVEC_ADDR)
	addu	k1, k1, k0
	lw		k0, 0(k1)
	nop
	/* If trap vector entry is null, go to the */
	/*  generic handler.                       */
	beq		k0, zero, savestate
	nop
	/* Else jump directly to exception handler.*/
	jr		k0
	nop
	.set reorder
		
/**
 * Save processor state and jump to main handler.  The IRQ Handler saves all
 * relevant volatile state on the stack, and transfers control to a C function
 * for decoding the interrupt source and picking the correct vector.
 */
savestate:						
	addiu	sp, sp, -IRQREC_SIZE		 	
		
	.set noreorder
	.set noat
	/* Save	AT (assembler temp) before any synthetic opcodes.	*/
	sw		AT, IRQREC_AT(sp)
	/* Save interrupt/exception CAUSE value.			*/
	mfc0	k0, CP0_CAUSE
	/* Save program counter from interrupt/exception source.	*/
	mfc0	k1, CP0_EPC
	sw		k0, IRQREC_CAUSE(sp)
	/* Save STATUS register from coprocessor.			*/
	mfc0	k0, CP0_STATUS
	sw		k1, IRQREC_EPC(sp)
	sw		k0, IRQREC_STATUS(sp)
	.set at
	.set reorder

	/* Save all general purpose registers.				*/
	sw  v0, IRQREC_V0(sp)          
	sw  v1, IRQREC_V1(sp)			
	sw  a0, IRQREC_A0(sp)			
	sw  a1, IRQREC_A1(sp)			
	sw  a2, IRQREC_A2(sp)			
	sw  a3, IRQREC_A3(sp)			
	sw  t0, IRQREC_T0(sp)			
	sw  t1, IRQREC_T1(sp)			
	sw  t2, IRQREC_T2(sp)		
	sw  t3, IRQREC_T3(sp)		
	sw  t4, IRQREC_T4(sp)		
	sw  t5, IRQREC_T5(sp)		
	sw  t6, IRQREC_T6(sp)		
	sw  t7, IRQREC_T7(sp)		
	sw  s0, IRQREC_S0(sp)		
	sw  s1, IRQREC_S1(sp)		
	sw  s2, IRQREC_S2(sp)		
	sw  s3, IRQREC_S3(sp)		
	sw  s4, IRQREC_S4(sp)		
	sw  s5, IRQREC_S5(sp)		
	sw  s6, IRQREC_S6(sp)		
	sw  s7, IRQREC_S7(sp)		
	sw  t8, IRQREC_T8(sp)		
	sw  t9, IRQREC_T9(sp)		
	sw  k0, IRQREC_K0(sp)		
	sw  k1, IRQREC_K1(sp)		
	sw  gp, IRQREC_GP(sp)		
	sw  sp, IRQREC_SP(sp)		
	sw  fp, IRQREC_FP(sp)		
	sw  ra, IRQREC_RA(sp)		

	/* Save a couple of other volatile regs, hi and lo.	*/
	mfhi	t0						
	mflo	t1							
	sw	t0, IRQREC_HI(sp)			
	sw	t1, IRQREC_LO(sp)			
	
	/* Call high-level trap handler.			*/
	lw	a0, IRQREC_CAUSE(sp)
	move	a1, sp
	jal	xtrap

/**
 * Common return routine for exceptions and interrupts.  All interrupts
 * that save state will eventually return here, where the state is restored
 * and control is returned to the point where the interrupt occurred.
 */
ret_from_exception:
	/* Restore HI and LO special regs. */
	lw  t0, IRQREC_HI(sp)			
	lw  t1, IRQREC_LO(sp)			
	mthi t0				
	mtlo t1						

	/* Restore general purpose regs. */
	lw  ra, IRQREC_RA(sp) 		
	lw  fp, IRQREC_FP(sp)		
	/* lw  sp, IRQREC_SP(sp) */
 	lw  gp, IRQREC_GP(sp)		
	/* lw  k1, IRQREC_K1(sp) */
 	/* lw  k0, IRQREC_K0(sp) */
 	lw  t9, IRQREC_T9(sp)		
 	lw  t8, IRQREC_T8(sp)		
 	lw  s7, IRQREC_S7(sp)		
 	lw  s6, IRQREC_S6(sp)		
 	lw  s5, IRQREC_S5(sp)		
 	lw  s4, IRQREC_S4(sp)		
 	lw  s3, IRQREC_S3(sp)		
 	lw  s2, IRQREC_S2(sp)		
 	lw  s1, IRQREC_S1(sp)		
 	lw  s0, IRQREC_S0(sp)		
 	lw  t7, IRQREC_T7(sp)		
 	lw  t6, IRQREC_T6(sp)		
 	lw  t5, IRQREC_T5(sp)		
 	lw  t4, IRQREC_T4(sp)		
 	lw  t3, IRQREC_T3(sp)		
 	lw  t2, IRQREC_T2(sp)		
 	lw  t1, IRQREC_T1(sp)			
 	lw  t0, IRQREC_T0(sp)			
 	lw  a3, IRQREC_A3(sp)			
 	lw  a2, IRQREC_A2(sp)			
 	lw  a1, IRQREC_A1(sp)			
 	lw  a0, IRQREC_A0(sp)			
 	lw  v1, IRQREC_V1(sp)			
 	lw  v0, IRQREC_V0(sp)			
 
	/* restore EPC */
	.set noreorder
	.set noat
	lw  k0, IRQREC_EPC(sp)
 	/* restore AT */
 	lw  AT, IRQREC_AT(sp)				
	mtc0 k0, CP0_EPC
	.set at
	.set reorder
		
	addiu sp, sp, IRQREC_SIZE			
	
	.set noreorder
	nop
	nop
	eret
	nop
	nop
	.set reorder
	.end irqhandler
