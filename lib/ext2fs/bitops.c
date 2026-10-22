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

#ضمن "config.h"
#ضمن <stdio.h>
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

صحيح ext2fs_set_bit(unsigned صحيح nr,فراغ * addr)
{
	صحيح		mask, retval;
	unsigned حرف	*ADDR = (unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ܦܢܐ retval;
}

صحيح ext2fs_clear_bit(unsigned صحيح nr, فراغ * addr)
{
	صحيح		mask, retval;
	unsigned حرف	*ADDR = (unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ܦܢܐ retval;
}

صحيح ext2fs_test_bit(unsigned صحيح nr, ثابت فراغ * addr)
{
	صحيح			mask;
	ثابت unsigned حرف	*ADDR = (ثابت unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ܦܢܐ (mask & *ADDR);
}

فراغ ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ثابت حرف *description)
{
#ifndef OMIT_COM_ERR
	ܐܢ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ܐܚܪܢܐ
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

صحيح ext2fs_set_bit64(__u64 nr, فراغ * addr)
{
	صحيح		mask, retval;
	unsigned حرف	*ADDR = (unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ܦܢܐ retval;
}

صحيح ext2fs_clear_bit64(__u64 nr, فراغ * addr)
{
	صحيح		mask, retval;
	unsigned حرف	*ADDR = (unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ܦܢܐ retval;
}

صحيح ext2fs_test_bit64(__u64 nr, ثابت فراغ * addr)
{
	صحيح			mask;
	ثابت unsigned حرف	*ADDR = (ثابت unsigned حرف *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ܦܢܐ (mask & *ADDR);
}

ساكن unsigned صحيح popcount8(unsigned صحيح w)
{
	unsigned صحيح res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ܦܢܐ (res + (res >> 4)) & 0x0F;
}

ساكن unsigned صحيح popcount32(unsigned صحيح w)
{
	unsigned صحيح res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ܦܢܐ (res + (res >> 16)) & 0x000000FF;
}

unsigned صحيح ext2fs_bitcount(ثابت فراغ *addr, unsigned صحيح nbytes)
{
	ثابت unsigned حرف *cp = addr;
	ثابت __u32 *p;
	unsigned صحيح res = 0;

	ܟܡܐ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ثابت __u32 *) cp;

	ܟܡܐ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ثابت unsigned حرف *) p;

	ܟܡܐ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ܦܢܐ res;
}
