/**
 * @file interrupt.h
 * 
 * Constants and declarations associated with interrupt and exception
 * processing.
 *
 * $Id: interrupt.h 178 2007-07-12 04:04:27Z mschul $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#ifndef INTERRUPT_H
#define INTERRUPT_H

/** Address of interrupt request handler.                          */
#define IRQ_ADDR      0x80000180
/** Address of XINU-defined trap (exception) vector.               */
#define TRAPVEC_ADDR  0x80000200
/** Address of XINU-defined interrupt request vector.              */
#define IRQVEC_ADDR   0x80000280
/** Address of end of XINU-defined interrupt tables.               */
#define IRQVEC_END    0x800002A0

/**
 * Definitions to allow C array manipulation of vectors.
 * The cast below makes the following a pointer to a table of
 * pointers to functions which take no parameters (void) and return
 * nothing (void).
 */
#define exceptionVector ((interrupt (**)(void))TRAPVEC_ADDR)
#define interruptVector ((interrupt (**)(void))IRQVEC_ADDR)

/* Indices for interrupt request vector.                           */
#define IRQ_SW0       0
#define IRQ_SW1       1
#define IRQ_HW0       2
#define IRQ_HW1       3
#define IRQ_HW2       4
#define IRQ_HW3       5
#define IRQ_HW4       6
#define IRQ_HW5       7

/* Names for known IRQ vectors                                     */
#define IRQ_TIMER     IRQ_HW5
#define IRQ_UART      IRQ_HW1
#define IRQ_ETH0      IRQ_HW2
#define IRQ_ETH1      IRQ_HW0

/* Offsets for the MIPS interrupt context record.                  */
#define IRQREC_ZER    24 
#define IRQREC_AT     28
#define IRQREC_V0     32
#define IRQREC_V1     36
#define IRQREC_A0     40
#define IRQREC_A1     44
#define IRQREC_A2     48
#define IRQREC_A3     52
#define IRQREC_T0     56
#define IRQREC_T1     60
#define IRQREC_T2     64
#define IRQREC_T3     68
#define IRQREC_T4     72
#define IRQREC_T5     76
#define IRQREC_T6     80
#define IRQREC_T7     84
#define IRQREC_S0     88
#define IRQREC_S1     92
#define IRQREC_S2     96
#define IRQREC_S3     100
#define IRQREC_S4     104
#define IRQREC_S5     108
#define IRQREC_S6     112
#define IRQREC_S7     116
#define IRQREC_T8     120
#define IRQREC_T9     124
#define IRQREC_K0     128
#define IRQREC_K1     132
#define IRQREC_GP     136
#define IRQREC_SP     140
#define IRQREC_FP     144
#define IRQREC_RA     148
#define IRQREC_LO     152
#define IRQREC_HI     156
#define IRQREC_EPC    160
#define IRQREC_CAUSE  164
#define IRQREC_STATUS 168	
#define IRQREC_SIZE   172

#endif /*! INTERRUPT_H */
