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

int prioritize(pid_typ pid, qid_typ q, ulong key)
{
	if (!isbadqueue(q) && !isbadpid(pid))
        {
		ulong head = queuehead(q);
                ulong currentindex = queuetab[queuetail(q)].prev;
		while((currentindex != head) && queuetab[currentindex].key < key)
                { 
			currentindex = queuetab[currentindex].prev;
                }
		queuetab[pid].prev = currentindex;
		queuetab[pid].next = queuetab[currentindex].next;
		queuetab[currentindex].next = pid;
		queuetab[queuetab[pid].next].prev = pid;
		queuetab[pid].key = key;
		return OK;
        }
        else
        {
                return SYSERR;
        }
}
