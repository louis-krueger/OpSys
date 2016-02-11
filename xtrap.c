/**
 * @file xtrap.c 
 * @provides xtrap
 *
 * $Id: xtrap.c 170 2007-07-11 18:45:46Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <mips.h>
#include <interrupt.h>

char *trap_names[] =
{ 
	"Int, Interrupt",
	"Mod, TLB modification exception",
	"TLBL, TLB exception (load or instruction fetch)",
	"TLBS, TLB exception (store)",
	"AdEL, Address error exception (load or instruction fetch)",
	"AdES, Address error exception (store)",
	"IBE, Bus error exception (instruction fetch)",
	"DBE, Bus error exception (data reference: load or store)",
	"Sys, Syscall exception",
	"Bp, Breakpoint exception",
	"RI, Reserved instruction exception",
	"CpU, Coprocessor Unusable exception",
	"Ov, Arithmetic Overflow exception",
	"Tr, Trap Exception",
	"Reserved",
	"FPE, Floating point exception",
	"Reserved",
	"Reserved",
	"C2E, Reserved for precise Coprocessor 2 exceptions",
	"Reserved",
	"Reserved",
	"Reserved",
	"MDMX, MDMX Unusuable in MIPS64",
	"WATCH, Reference to WatchHi/WatchLo address",
	"MCheck, Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"CacheErr, Cache error / Debug Mode",
	"Reserved"
};

char *irq_names[] =
{ 
	"Software interrupt request 0",
	"Software interrupt request 1",
	"Hardware interrupt request 0, eth1",
	"Hardware interrupt request 1, UART",
	"Hardware interrupt request 2, eth0",
	"Hardware interrupt request 3",
	"Hardware interrupt request 4",
	"Hardware interrupt request 5, Timer"
};

/**
 * Generic XINU Trap (Interrupt) Handler.
 *
 * @param cause the contents of the cause register used to decode the trap
 * @param frame pointer to the interrupt frame with saved status
 *
 */
void xtrap(long cause, long *frame)
{
	long exccode = 0, irqcode = 0, irqnum = -1;
	void (*handler)(void);

	exccode = (cause >> 2) & 0x0000001F;
	irqcode = (cause >> 8) & 0x000000FF;
  
	if (exccode)
	{
		kprintf("XINU Exception 0x%04X, %s\r\n", cause, trap_names[exccode]);
	}
	else
	{
		/* Calculate which interrupt number this is. */
		while (irqcode)
		{ irqnum++;  irqcode = irqcode >> 1; }
		/* Check for registered interrupt handler. */
		if ( NULL != (handler = interruptVector[irqnum]) )
		{
			(*handler)();
			return; /* return through ret_from_exception in irq.S */
		}
		kprintf("XINU Trap %d, %s\r\n", irqnum, irq_names[irqnum]);
	}

	kprintf("Faulting address: 0x%08X\r\n", frame[(IRQREC_EPC) / 4]);

	kprintf("[0x%08X] sta:0x%08X\r\n",
			frame+36, frame[IRQREC_STATUS/4]);
	kprintf("[0x%08X]  lo:0x%08X   hi:0x%08X  epc:0x%08X  cau:0x%08X\r\n",
			frame+32, frame[IRQREC_LO/4], frame[IRQREC_HI/4],
			frame[IRQREC_EPC/4], frame[IRQREC_CAUSE/4]);
	kprintf("[0x%08X]  gp:0x%08X   sp:0x%08X   fp:0x%08X   ra:0x%08X\r\n",
			frame+28, frame[IRQREC_GP/4], frame[IRQREC_SP/4],
			frame[IRQREC_FP/4], frame[IRQREC_RA/4]);
	kprintf("[0x%08X]  t8:0x%08X   t9:0x%08X   k0:0x%08X   k1:0x%08X\r\n",
			frame+24, frame[IRQREC_T8/4], frame[IRQREC_T9/4],
			frame[IRQREC_K0/4], frame[IRQREC_K1/4]);
	kprintf("[0x%08X]  s4:0x%08X   s5:0x%08X   s6:0x%08X   s7:0x%08X\r\n",
			frame+20, frame[IRQREC_S4/4], frame[IRQREC_S5/4],
			frame[IRQREC_S6/4], frame[IRQREC_S7/4]);
	kprintf("[0x%08X]  s0:0x%08X   s1:0x%08X   s2:0x%08X   s3:0x%08X\r\n",
			frame+16, frame[IRQREC_S0/4], frame[IRQREC_S1/4],
			frame[IRQREC_S2/4], frame[IRQREC_S3/4]);
	kprintf("[0x%08X]  t4:0x%08X   t5:0x%08X   t6:0x%08X   t7:0x%08X\r\n",
			frame+12, frame[IRQREC_T4/4], frame[IRQREC_T5/4],
			frame[IRQREC_T6/4], frame[IRQREC_T7/4]);
	kprintf("[0x%08X]  t0:0x%08X   t1:0x%08X   t2:0x%08X   t3:0x%08X\r\n",
			frame+8, frame[IRQREC_T0/4], frame[IRQREC_T1/4],
			frame[IRQREC_T2/4], frame[IRQREC_T3/4]);
	kprintf("[0x%08X]  a0:0x%08X   a1:0x%08X   a2:0x%08X   a3:0x%08X\r\n",
			frame+4, frame[IRQREC_A0/4], frame[IRQREC_A1/4],
			frame[IRQREC_A2/4], frame[IRQREC_A3/4]);
	kprintf("[0x%08X] zer:0x%08X   at:0x%08X   v0:0x%08X   v1:0x%08X\r\n",
			frame+0, frame[IRQREC_ZER/4], frame[IRQREC_AT/4],
			frame[IRQREC_V0/4], frame[IRQREC_V1/4]);
	
	while (1)
		; /* forever */
}

