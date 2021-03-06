/**
 * @file mutex.c
 * @provides mutexInit, mutexAcquire, mutexRelease
 * TA-BOT:MAILTO samuel.scheel@marquette.edu louis.krueger@marquette.edu
 * Modified by
 * Samuel Scheel
 * and
 * Louis Krueger
 */

#include <xinu.h>

static volatile bool waiting[NPROC];
static volatile bool lock;
/**
 * Initialize bounded-waiting mutex subsystem.
 */
void mutexInit(void)
{
    int i = 0;
    for (i = 0; i < NPROC; i++)
        waiting[i] = FALSE;
    lock = FALSE;
}

/**
 * Acquire lock using MIPS testAndSet.
 * See textbook Figure 5.7, page 211.
 */
void mutexAcquire(void)
{
    bool key = TRUE;
    // TODO:
    // Implement mutex acquire using ARM testAndSet().
    // This function will loop until it acquires the lock.
    waiting[currpid] = TRUE;
    key = TRUE;
    while (waiting[currpid] && key)
    {
        key = testAndSet(&lock);
    }
    waiting[currpid] = FALSE;
}

/**
 * Release acquired lock, fairly select next proc to enter critical section.
 * See textbook Figure 5.7, page 211.
 */
void mutexRelease(void)
{
    int j;
    j = (currpid + 1) % NPROC;
    // TODO:
    // Implement mutex release and select the next process
    //   to admit into its critical section.
    while ((j != currpid) && !waiting[j])
    {
        j = (j + 1) % NPROC;
    }
    if (j == currpid)
	lock = FALSE;
    else
	waiting[j] = FALSE;
}
