/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * libcglue.c - Newlib-compatible system calls.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 * Copyright (c) 2005 Jim Paris <jim@jtan.com>
 * Copyright (c) 2021 Francisco J Trujillo <fjtrujy@gmail.com>
 * 
 */

#include <stdint.h>

#ifdef F__lock_unlock

static unsigned int lock_count = 0;
static unsigned int intr_flags = 0;


void __malloc_lock(struct _reent *ptr)
{
	unsigned int flags = _platformDisableInterrupts();

	if (lock_count == 0) {
		intr_flags = flags;
	}

	lock_count++;
}

void __malloc_unlock(struct _reent *ptr)
{
	if (--lock_count == 0) {
		_platformEnableInterrupts(intr_flags);
	}
}
#endif
