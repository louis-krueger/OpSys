/**
 * @file startup.S
 * @provides _startup
 *
 * $Id: startup.S 184 2007-07-12 22:52:09Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
 * This is where kernel execution begins once loader hands off.  The boot
 * loader flushes caches, copies this kernel's interrupt handler to proper
 * location in memory and hands off control.
 */

#include <mips.h>
	
	.text
	.align 4

	.globl _startup

/**
 * This function sets up the expected C environment and then runs the null 
 * process.  A major part of the expected C environment is memory 
 * preparation.  The standard XINU executable consists of three main 
 * segments.  (In this context, a 'segment' is a relocatable section, not
 * necessarily corresponding to virtual memory segments.)  These are:
 *  Text segment (assembled machine instructions for the program code),
 *  Data segment (initialized data), and
 *  BSS ("Block Started by Symbol") segment (uninitialized data).
 * The Stack segment provides space for a stack of activation records
 *  used to implement statically-scoped variables and recursive procedure
 *  calls in most ALGOL-like languages, including C and all its progeny.
 * The BSS is the unitialized data segment produced by the Unix linkers.
 *  Objects in the BSS segment have only a name and a size, but no value.
 * In addition, the running program conceptually also has:	
 *  Stack segment (stack of activation records,)
 *  Heap segment (area for dynamic memory allocation).
 *
 * Memory Layout (Not to scale)
 * +----------+
 * |          |
 * |          |
 * |   HEAP   | -> getmem allocates from here
 * |          |
 * |          |
 * +----------+
 * | OS STACK | -> becomes the null process' stack
 * +----------+
 * |   BSS    | -> needed for C environment
 * +----------+
 * |   TEXT   | -> XINU code
 * |          |
 * +----------+
 * | RESERVED | -> interrupt handler and vectors go here
 * +----------+
 *
 * @param a0 processor ID
 * @param a1 maximum contiguous physical memory address
 * @param a2 time base frequency
 */
_startup:
	/* Store processor ID */
	move  s0, a0
	
	/* Setup the three main memory segments (see function summary) */
	
	/* Set up Stack segment (see function summary) */
	li    s1, 1024    /* Stack should be 32K */
	li    s2, 32
	mul   s1, s1, s2 
	la	  a0, end
	addu  s1, s1, a0  /* Top of stack is 32K from end of kernel image */

	/* Word align the top of the stack */
	subu  s1, s1, 1
	srl   s1, 4
	sll   s1, 4
	
	/* Initialize the stack and frame pointers */
	move  sp, s1
	move  fp, s1
	
	/* Zero out 32K of stack space below new stack pointer */
	subu   a1, s1, a0  
	jal   bzero 
	nop 
	/**
	 * Note: bzero routine must be optimized leaf function without
	 * activation record, or this will zero its own return address. 
	 */

	/* Set up BSS segment (see function summary). */
	la    a0, bss
	la    a1, end
	sub   a1, a0
	jal   bzero     /* Clear BSS Segment */
	nop

	/* Store processor ID */
	la    t0, cpuid
	sw    s0, 0(t0)

	/* Store bottom of the heap */
	la    t0, minheap
	sw    s1, 0(t0)
	
	j     nulluser /* start the null user process */
	nop

