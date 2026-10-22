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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

পূর্ণসংখ্যা ext2fs_set_bit(unsigned পূর্ণসংখ্যা nr,শূন্য * addr)
{
	পূর্ণসংখ্যা		mask, retval;
	unsigned অক্ষর	*ADDR = (unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_clear_bit(unsigned পূর্ণসংখ্যা nr, শূন্য * addr)
{
	পূর্ণসংখ্যা		mask, retval;
	unsigned অক্ষর	*ADDR = (unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_test_bit(unsigned পূর্ণসংখ্যা nr, ধ্রুবক শূন্য * addr)
{
	পূর্ণসংখ্যা			mask;
	ধ্রুবক unsigned অক্ষর	*ADDR = (ধ্রুবক unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ফেরত (mask & *ADDR);
}

শূন্য ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ধ্রুবক অক্ষর *description)
{
#ifndef OMIT_COM_ERR
	যদি (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	অন্যথা
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

পূর্ণসংখ্যা ext2fs_set_bit64(__u64 nr, শূন্য * addr)
{
	পূর্ণসংখ্যা		mask, retval;
	unsigned অক্ষর	*ADDR = (unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_clear_bit64(__u64 nr, শূন্য * addr)
{
	পূর্ণসংখ্যা		mask, retval;
	unsigned অক্ষর	*ADDR = (unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_test_bit64(__u64 nr, ধ্রুবক শূন্য * addr)
{
	পূর্ণসংখ্যা			mask;
	ধ্রুবক unsigned অক্ষর	*ADDR = (ধ্রুবক unsigned অক্ষর *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ফেরত (mask & *ADDR);
}

স্থির unsigned পূর্ণসংখ্যা popcount8(unsigned পূর্ণসংখ্যা w)
{
	unsigned পূর্ণসংখ্যা res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ফেরত (res + (res >> 4)) & 0x0F;
}

স্থির unsigned পূর্ণসংখ্যা popcount32(unsigned পূর্ণসংখ্যা w)
{
	unsigned পূর্ণসংখ্যা res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ফেরত (res + (res >> 16)) & 0x000000FF;
}

unsigned পূর্ণসংখ্যা ext2fs_bitcount(ধ্রুবক শূন্য *addr, unsigned পূর্ণসংখ্যা nbytes)
{
	ধ্রুবক unsigned অক্ষর *cp = addr;
	ধ্রুবক __u32 *p;
	unsigned পূর্ণসংখ্যা res = 0;

	যখন (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ধ্রুবক __u32 *) cp;

	যখন (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ধ্রুবক unsigned অক্ষর *) p;

	যখন (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ফেরত res;
}
