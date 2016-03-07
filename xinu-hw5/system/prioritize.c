/**
 * Written by Samuel Scheel & Louis Krueger
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * @file queue.c
 * @provides enqueue, remove, dequeue, getfirst, newqueue
 *
 * $Id: queue.c 189 2007-07-13 21:43:45Z brylow $
 *
 *Embedded XINU, Copyright (C) 2007.  All rights reserved. 
 */

#include <xinu.h>

//#define DEBUG 1

qid_typ printqueue(qid_typ);

qid_typ prioritize(pid_typ pid, qid_typ q, ulong key)
{
        if (!isbadqueue(q) && !isbadpid(pid))
        {
                #ifdef DEBUG
                        printqueue(q);
                #endif
                ulong head = queuehead(q);
                ulong currentindex = queuetail(q);
//		while(queuetab[currentindex].key < key)
  //              {
    //            	currentindex = queuetab[currentindex].prev;
      //          }
		queuetab[pid].next = currentindex;
		queuetab[pid].prev = queuetab[currentindex].prev;
		queuetab[queuetab[currentindex].prev].next = pid;
		queuetab[currentindex].prev = pid;
		queuetab[pid].key = key;
		return q;


/*

		//ulong currentindex = queuetab[queuetail(q)].prev;
		//queuetab[queuetab[currentindex].prev].next = EMPTY;
		while(currentindex != head && queuetab[currentindex].key < key)
                {
                        currentindex = queuetab[currentindex].prev;
                }
                queuetab[pid].next = currentindex;
                queuetab[pid].prev = queuetab[currentindex].prev;
                queuetab[queuetab[currentindex].prev].next = pid;
                queuetab[currentindex].prev = pid;
                queuetab[pid].key = key;
*/		//return OK;
        }
        else
        {
                return SYSERR;
        }
}
#ifdef DEBUG
        qid_typ printqueue(qid_typ q)
        {
                ulong current = queuetail(q);
                ulong tail = queuetail(q);
                ulong head = queuehead(q);
                kprintf("head: %u\r\n", queuehead(q));
                kprintf("tail: %u\r\n", queuetail(q));
                kprintf("entering loop\r\n");
               // while(current != head)
                //{
                        kprintf("current.prev: %u\r\n", queuetab[current].prev);
                        kprintf("current process: %u\r\n", current);
                        kprintf("\tname: %s\r\n", proctab[currpid].name);
                        kprintf("\tpid: %u\r\n", currpid);
                        kprintf("\tpriority: %u\r\n", proctab[currpid].priority);
                        kprintf("\tnumber of procs: %u\r\n", numproc);
                        kprintf("\tstate: %u\r\n", proctab[currpid].state);
                        kprintf("current.next: %u\r\n\r\n", queuetab[current].next);
                   //     current = queuetab[current].prev;
                //}
        	return OK;
        }
#endif
                                                         
