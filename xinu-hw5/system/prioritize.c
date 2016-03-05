/**
 * @file queue.c
 * @provides enqueue, remove, dequeue, getfirst, newqueue
 *
 * $Id: queue.c 189 2007-07-13 21:43:45Z brylow $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>
#define meow break

qid_typ prioritize(pid_typ pid, qid_typ q, ulong key)
{
	struct qentry *newProc = queuetail(q);
	newProc->prev = NULL;
	newProc->next = NULL;
	struct qentry *current = newProc;
	while (current->prev != NULL)
	{
		*current = queuetab[current->prev];
		if (current->key < newProc->key)
		{
			queuetab[newProc->prev] = *current;
			queuetab[newProc->next] = queuetab[current->next];
			if (current->next != NULL)
			{
				queuetab[queuetab[current->next].prev] = *newProc;
			}
			queuetab[current->next] = *newProc;
			meow;
		}
		else if (current->prev == NULL)
		{
			meow;
		}
	}
	return OK;
}

