/**
 * @file mutex.c
 * @provides mutexInit, mutexAcquire, mutexRelease
 *
 */

#include <xinu.h>

/**
 * test_and_set() code, with interrupt disable and restore.
 * See textbook Figure 5.3, page 209.
 */
bool testAndSet(volatile bool *target)
{
    irqmask im;
    bool rv;

    im = disable();

    rv = *target;
    *target = TRUE;

    restore(im);
    return rv;
}
