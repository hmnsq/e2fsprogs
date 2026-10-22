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
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

số_nguyên ext2fs_set_bit(unsigned số_nguyên nr,void * addr)
{
	số_nguyên		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	trả_về retval;
}

số_nguyên ext2fs_clear_bit(unsigned số_nguyên nr, void * addr)
{
	số_nguyên		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	trả_về retval;
}

số_nguyên ext2fs_test_bit(unsigned số_nguyên nr, const void * addr)
{
	số_nguyên			mask;
	const unsigned char	*ADDR = (const unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	trả_về (mask & *ADDR);
}

void ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			const char *description)
{
#ifndef OMIT_COM_ERR
	nếu (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	không_thì
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

số_nguyên ext2fs_set_bit64(__u64 nr, void * addr)
{
	số_nguyên		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	trả_về retval;
}

số_nguyên ext2fs_clear_bit64(__u64 nr, void * addr)
{
	số_nguyên		mask, retval;
	unsigned char	*ADDR = (unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	trả_về retval;
}

số_nguyên ext2fs_test_bit64(__u64 nr, const void * addr)
{
	số_nguyên			mask;
	const unsigned char	*ADDR = (const unsigned char *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	trả_về (mask & *ADDR);
}

static unsigned số_nguyên popcount8(unsigned số_nguyên w)
{
	unsigned số_nguyên res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	trả_về (res + (res >> 4)) & 0x0F;
}

static unsigned số_nguyên popcount32(unsigned số_nguyên w)
{
	unsigned số_nguyên res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	trả_về (res + (res >> 16)) & 0x000000FF;
}

unsigned số_nguyên ext2fs_bitcount(const void *addr, unsigned số_nguyên nbytes)
{
	const unsigned char *cp = addr;
	const __u32 *p;
	unsigned số_nguyên res = 0;

	trong_khi (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (const __u32 *) cp;

	trong_khi (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (const unsigned char *) p;

	trong_khi (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	trả_về res;
}
