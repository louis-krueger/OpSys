/**
 * @file debug.h
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stddef.h>

#define DEBUG_ASCII 0x01
#define DEBUG_HEX   0x02

void hexdump(void *buffer, ulong length, bool text);
void debugbreak(void);
void debugret(void);
int printqueue(qid_typ q);
int printcurrentprocess(pid_typ procid);
                          /* _DEBGU_H_ */
        int printqueue(qid_typ q)
        {
                ulong current = queuetab[queuehead(q)].next;
                ulong tail = queuetail(q);
                kprintf("\n\rhead: %u\r\n", queuehead(q));
                kprintf("tail: %u\r\n", queuetail(q));
                kprintf("***Queue iteam***\r\n");
                while(current != tail)
                {
                        kprintf("current.prev: %u\r\n", queuetab[current].prev);
                        kprintf("current process: %u\r\n", current);
                        kprintf("\tname: %s\r\n", proctab[current].name);
                        kprintf("\tpid: %u\r\n", current);
                        kprintf("\tpriority: %u\r\n", proctab[current].priority);
                        kprintf("\tnumber of procs: %u\r\n", numproc);
                        kprintf("\tstate: %u\r\n", proctab[current].state);
                        kprintf("current.next: %u\r\n", queuetab[current].next);
                        current = queuetab[current].next;
                }
		kprintf("***End of queue items \r\n\r\n");
                return OK;
        }

	int printcurrentprocess(pid_typ procid)
	{	
		pcb *ppcb = &proctab[procid];
		kprintf("\n***Current process info***\n\r");
		kprintf("process name:%s\n\r", ppcb->name);
                kprintf("proc state:%d\n\r", ppcb->state);    //ppcb->state same thing as (*ppcb).state
                kprintf("stack base address:0x%08X\n\r", ppcb->stkbase);
                kprintf("stack length:0x%08X\n\r", ppcb->stklen);			
		return OK;
	}


#endif

