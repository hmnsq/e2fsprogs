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
#གལ་སྲིད HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ཧྲིལ ext2fs_set_bit(unsigned ཧྲིལ nr,void * addr)
{
	ཧྲིལ		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ལོག retval;
}

ཧྲིལ ext2fs_clear_bit(unsigned ཧྲིལ nr, void * addr)
{
	ཧྲིལ		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ལོག retval;
}

ཧྲིལ ext2fs_test_bit(unsigned ཧྲིལ nr, const void * addr)
{
	ཧྲིལ			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ལོག (mask & *ADDR);
}

void ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			const var_na *description)
{
#ifndef OMIT_COM_ERR
	གལ་སྲིད (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	དེ་མིན
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

ཧྲིལ ext2fs_set_bit64(__u64 nr, void * addr)
{
	ཧྲིལ		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ལོག retval;
}

ཧྲིལ ext2fs_clear_bit64(__u64 nr, void * addr)
{
	ཧྲིལ		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ལོག retval;
}

ཧྲིལ ext2fs_test_bit64(__u64 nr, const void * addr)
{
	ཧྲིལ			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ལོག (mask & *ADDR);
}

static unsigned ཧྲིལ popcount8(unsigned ཧྲིལ w)
{
	unsigned ཧྲིལ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ལོག (res + (res >> 4)) & 0x0F;
}

static unsigned ཧྲིལ popcount32(unsigned ཧྲིལ w)
{
	unsigned ཧྲིལ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ལོག (res + (res >> 16)) & 0x000000FF;
}

unsigned ཧྲིལ ext2fs_bitcount(const void *addr, unsigned ཧྲིལ nbytes)
{
	const unsigned var_na *cp = addr;
	const __u32 *p;
	unsigned ཧྲིལ res = 0;

	སྐབས (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (const __u32 *) cp;

	སྐབས (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (const unsigned var_na *) p;

	སྐབས (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ལོག res;
}
