#ifndef  _ARM_H_
#define  _ARM_H_

/* Definitions of mode bits in the ARM program status register.  See: A2.2
 * "Processor Modes" of the ARM Architecture Reference Manual; also A2.5.7 "The
 * mode bits".  */
#define ARM_MODE_USR 0x10       /* Normal User Mode                                       */
#define ARM_MODE_FIQ 0x11       /* FIQ Processing Fast Interrupts Mode                    */
#define ARM_MODE_IRQ 0x12       /* IRQ Processing Standard Interrupts Mode                */
#define ARM_MODE_SVC 0x13       /* Supervisor Processing Software Interrupts Mode         */
#define ARM_MODE_ABT 0x17       /* Abort Processing memory Faults Mode                    */
#define ARM_MODE_UND 0x1B       /* Undefined Processing Undefined Instructions Mode       */
#define ARM_MODE_SYS 0x1F       /* System Running Priviledged Operating System Tasks Mode */

/* Definitions of interrupt disable bits in the ARM program status register.
 * See: A2.5.6 "The interrupt disable bits" of the ARM Architecture Reference
 * Manual.  */
#define ARM_I_BIT 0x80          /* IRQs disabled when set to 1. */
#define ARM_F_BIT 0x40          /* FIQs disabled when set to 1. */


/**
* Process Control Block register constants.
* These are the indices of registers stored in a PCB.
*/

#define PREG_R0     0           /* Argument/return/scratch registers             */
#define PREG_R1     1
#define PREG_R2     2           /* Argument/scratch registers                    */
#define PREG_R3     3
#define PREG_R4     4           /* Callee-save regs                              */
#define PREG_R5     5
#define PREG_R6     6
#define PREG_R7     7
#define PREG_R8     8
#define PREG_R9     9
#define PREG_R10    10
#define PREG_R11    11
#define PREG_IP     12          /* Intra-procedural scratch register             */
#define PREG_SP     13          /* Stack pointer                                 */
#define PREG_LR     14          /* Link register                                 */
#define PREG_PC     15          /* Program counter                               */

#define PREGS       16          /* Regs stored by context switches               */


#endif                          /* _ARM_H_ */
