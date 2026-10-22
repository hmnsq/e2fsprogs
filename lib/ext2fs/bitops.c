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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

પૂર્ણાંક ext2fs_set_bit(unsigned પૂર્ણાંક nr,શૂન્ય * addr)
{
	પૂર્ણાંક		mask, retval;
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	પાછા retval;
}

પૂર્ણાંક ext2fs_clear_bit(unsigned પૂર્ણાંક nr, શૂન્ય * addr)
{
	પૂર્ણાંક		mask, retval;
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	પાછા retval;
}

પૂર્ણાંક ext2fs_test_bit(unsigned પૂર્ણાંક nr, અચલ શૂન્ય * addr)
{
	પૂર્ણાંક			mask;
	અચલ unsigned અક્ષર	*ADDR = (અચલ unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	પાછા (mask & *ADDR);
}

શૂન્ય ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			અચલ અક્ષર *description)
{
#ifndef OMIT_COM_ERR
	જો (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	નહીંતર
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

પૂર્ણાંક ext2fs_set_bit64(__u64 nr, શૂન્ય * addr)
{
	પૂર્ણાંક		mask, retval;
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	પાછા retval;
}

પૂર્ણાંક ext2fs_clear_bit64(__u64 nr, શૂન્ય * addr)
{
	પૂર્ણાંક		mask, retval;
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	પાછા retval;
}

પૂર્ણાંક ext2fs_test_bit64(__u64 nr, અચલ શૂન્ય * addr)
{
	પૂર્ણાંક			mask;
	અચલ unsigned અક્ષર	*ADDR = (અચલ unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	પાછા (mask & *ADDR);
}

સ્થિર unsigned પૂર્ણાંક popcount8(unsigned પૂર્ણાંક w)
{
	unsigned પૂર્ણાંક res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	પાછા (res + (res >> 4)) & 0x0F;
}

સ્થિર unsigned પૂર્ણાંક popcount32(unsigned પૂર્ણાંક w)
{
	unsigned પૂર્ણાંક res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	પાછા (res + (res >> 16)) & 0x000000FF;
}

unsigned પૂર્ણાંક ext2fs_bitcount(અચલ શૂન્ય *addr, unsigned પૂર્ણાંક nbytes)
{
	અચલ unsigned અક્ષર *cp = addr;
	અચલ __u32 *p;
	unsigned પૂર્ણાંક res = 0;

	જ્યારે (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (અચલ __u32 *) cp;

	જ્યારે (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (અચલ unsigned અક્ષર *) p;

	જ્યારે (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	પાછા res;
}
