/*
 * bitops.c --- Bitmap frobbing code.  See bitops.h for the inlined
 * 	routines.
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#ប្រសិនបើ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ចំនួនគត់ ext2fs_set_bit(unsigned ចំនួនគត់ nr,void * addr)
{
	ចំនួនគត់		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ត្រឡប់ retval;
}

ចំនួនគត់ ext2fs_clear_bit(unsigned ចំនួនគត់ nr, void * addr)
{
	ចំនួនគត់		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ត្រឡប់ retval;
}

ចំនួនគត់ ext2fs_test_bit(unsigned ចំនួនគត់ nr, const void * addr)
{
	ចំនួនគត់			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ត្រឡប់ (mask & *ADDR);
}

void ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			const var_na *description)
{
#ifndef OMIT_COM_ERR
	ប្រសិនបើ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	បើមិនដូច្នេះ
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

ចំនួនគត់ ext2fs_set_bit64(__u64 nr, void * addr)
{
	ចំនួនគត់		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ត្រឡប់ retval;
}

ចំនួនគត់ ext2fs_clear_bit64(__u64 nr, void * addr)
{
	ចំនួនគត់		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ត្រឡប់ retval;
}

ចំនួនគត់ ext2fs_test_bit64(__u64 nr, const void * addr)
{
	ចំនួនគត់			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ត្រឡប់ (mask & *ADDR);
}

static unsigned ចំនួនគត់ popcount8(unsigned ចំនួនគត់ w)
{
	unsigned ចំនួនគត់ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ត្រឡប់ (res + (res >> 4)) & 0x0F;
}

static unsigned ចំនួនគត់ popcount32(unsigned ចំនួនគត់ w)
{
	unsigned ចំនួនគត់ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ត្រឡប់ (res + (res >> 16)) & 0x000000FF;
}

unsigned ចំនួនគត់ ext2fs_bitcount(const void *addr, unsigned ចំនួនគត់ nbytes)
{
	const unsigned var_na *cp = addr;
	const __u32 *p;
	unsigned ចំនួនគត់ res = 0;

	ខណៈពេល (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (const __u32 *) cp;

	ខណៈពេល (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (const unsigned var_na *) p;

	ខណៈពេល (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ត្រឡប់ res;
}
