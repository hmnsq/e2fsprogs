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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

صحیح ext2fs_set_bit(unsigned صحیح nr,تهی * addr)
{
	صحیح		mask, retval;
	unsigned نویسه	*ADDR = (unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	بازگشت retval;
}

صحیح ext2fs_clear_bit(unsigned صحیح nr, تهی * addr)
{
	صحیح		mask, retval;
	unsigned نویسه	*ADDR = (unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	بازگشت retval;
}

صحیح ext2fs_test_bit(unsigned صحیح nr, ثابت تهی * addr)
{
	صحیح			mask;
	ثابت unsigned نویسه	*ADDR = (ثابت unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	بازگشت (mask & *ADDR);
}

تهی ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ثابت نویسه *description)
{
#ifndef OMIT_COM_ERR
	اگر (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	وگرنه
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

صحیح ext2fs_set_bit64(__u64 nr, تهی * addr)
{
	صحیح		mask, retval;
	unsigned نویسه	*ADDR = (unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	بازگشت retval;
}

صحیح ext2fs_clear_bit64(__u64 nr, تهی * addr)
{
	صحیح		mask, retval;
	unsigned نویسه	*ADDR = (unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	بازگشت retval;
}

صحیح ext2fs_test_bit64(__u64 nr, ثابت تهی * addr)
{
	صحیح			mask;
	ثابت unsigned نویسه	*ADDR = (ثابت unsigned نویسه *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	بازگشت (mask & *ADDR);
}

ایستا unsigned صحیح popcount8(unsigned صحیح w)
{
	unsigned صحیح res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	بازگشت (res + (res >> 4)) & 0x0F;
}

ایستا unsigned صحیح popcount32(unsigned صحیح w)
{
	unsigned صحیح res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	بازگشت (res + (res >> 16)) & 0x000000FF;
}

unsigned صحیح ext2fs_bitcount(ثابت تهی *addr, unsigned صحیح nbytes)
{
	ثابت unsigned نویسه *cp = addr;
	ثابت __u32 *p;
	unsigned صحیح res = 0;

	وقتی (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ثابت __u32 *) cp;

	وقتی (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ثابت unsigned نویسه *) p;

	وقتی (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	بازگشت res;
}
