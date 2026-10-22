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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_set_bit(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr,ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		mask, retval;
	unsigned ଅକ୍ଷର	*ADDR = (unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_clear_bit(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr, ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		mask, retval;
	unsigned ଅକ୍ଷର	*ADDR = (unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_bit(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr, ଅଚଳ ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			mask;
	ଅଚଳ unsigned ଅକ୍ଷର	*ADDR = (ଅଚଳ unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ଫେରନ୍ତୁ (mask & *ADDR);
}

ଶୂନ୍ୟ ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ଅଚଳ ଅକ୍ଷର *description)
{
#ifndef OMIT_COM_ERR
	ଯଦି (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ଅନ୍ୟଥା
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_set_bit64(__u64 nr, ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		mask, retval;
	unsigned ଅକ୍ଷର	*ADDR = (unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_clear_bit64(__u64 nr, ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		mask, retval;
	unsigned ଅକ୍ଷର	*ADDR = (unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_bit64(__u64 nr, ଅଚଳ ଶୂନ୍ୟ * addr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			mask;
	ଅଚଳ unsigned ଅକ୍ଷର	*ADDR = (ଅଚଳ unsigned ଅକ୍ଷର *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ଫେରନ୍ତୁ (mask & *ADDR);
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା popcount8(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା w)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ଫେରନ୍ତୁ (res + (res >> 4)) & 0x0F;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା popcount32(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା w)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ଫେରନ୍ତୁ (res + (res >> 16)) & 0x000000FF;
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_bitcount(ଅଚଳ ଶୂନ୍ୟ *addr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbytes)
{
	ଅଚଳ unsigned ଅକ୍ଷର *cp = addr;
	ଅଚଳ __u32 *p;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	ଯେତେବେଳେ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ଅଚଳ __u32 *) cp;

	ଯେତେବେଳେ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ଅଚଳ unsigned ଅକ୍ଷର *) p;

	ଯେତେବେଳେ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ଫେରନ୍ତୁ res;
}
