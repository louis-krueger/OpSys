/**
 * @file     xinu.h
 *
 * Provides all of the #includes in standard order for easy inclusion.
 *
 */
/* Embedded XINU, Copyright (C) 2012.  All rights reserved. */

#include <kernel.h>
#include <platform.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <mips.h>
#include <proc.h>
#include <interrupt.h>
#include <device.h>
#include <queue.h>
#include <semaphore.h>
#include <bufpool.h>
#include <clock.h>
#include <memory.h>
#include <nvram.h>

#include <sbchipc.h>
#include <sbconfig.h>
#include <backplane.h>
#include <bcm4713.h>
#include <bcmswitch.h>
#include <ether.h>
#include <network.h>
#include <vlan.h>

#include <uart.h>
#include <tty.h>
#include <disk.h>
#include <file.h>
#include <shell.h>
