/**
 * @file queue.c
 * @provides enqueue, remove, dequeue, getfirst, newqueue
 *
 * $Id: queue.c 189 2007-07-13 21:43:45Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>
#define DEBUG 2

qid_typ prioritize(pid_typ pid, qid_typ q, ulong key)
{
	struct qentry newProc = queuetab[pid];
	newProc.key = key;
	#ifdef DEBUG
		kprintf("Key: %u\n\r", key);
	#endif	
	struct qentry current = newProc;
	#ifdef DEBUG
		kprintf("new proc info\r\n");
		kprintf("proc pid: %u\r\n", pid);
		kprintf("next: %u\r\n", newProc.next);
		kprintf("prev: %u\r\n", newProc.prev);
	#endif

	while (current.prev != EMPTY)
	{
		current = queuetab[current.prev];
		#ifdef DEBUG
			kprintf("current proc info\r\n");
			kprintf("next: %u\r\n", current.next);
			kprintf("prev: %u\r\n", current.prev);
		#endif
		if (current.key < newProc.key)
		{
			queuetab[current.next] = queuetab[newProc.next];
			queuetab[newProc.prev] = queuetab[current.prev];
			queuetab[newProc.next] = current;
			queuetab[current.prev] = newProc;
		}
	}
	return q;
}

