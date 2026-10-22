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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

முழு ext2fs_set_bit(unsigned முழு nr,வெறுமை * addr)
{
	முழு		mask, retval;
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	திரும்பு retval;
}

முழு ext2fs_clear_bit(unsigned முழு nr, வெறுமை * addr)
{
	முழு		mask, retval;
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	திரும்பு retval;
}

முழு ext2fs_test_bit(unsigned முழு nr, மாறிலி வெறுமை * addr)
{
	முழு			mask;
	மாறிலி unsigned எழுத்து	*ADDR = (மாறிலி unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	திரும்பு (mask & *ADDR);
}

வெறுமை ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			மாறிலி எழுத்து *description)
{
#ifndef OMIT_COM_ERR
	எனில் (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	இல்லையெனில்
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

முழு ext2fs_set_bit64(__u64 nr, வெறுமை * addr)
{
	முழு		mask, retval;
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	திரும்பு retval;
}

முழு ext2fs_clear_bit64(__u64 nr, வெறுமை * addr)
{
	முழு		mask, retval;
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	திரும்பு retval;
}

முழு ext2fs_test_bit64(__u64 nr, மாறிலி வெறுமை * addr)
{
	முழு			mask;
	மாறிலி unsigned எழுத்து	*ADDR = (மாறிலி unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	திரும்பு (mask & *ADDR);
}

நிலையான unsigned முழு popcount8(unsigned முழு w)
{
	unsigned முழு res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	திரும்பு (res + (res >> 4)) & 0x0F;
}

நிலையான unsigned முழு popcount32(unsigned முழு w)
{
	unsigned முழு res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	திரும்பு (res + (res >> 16)) & 0x000000FF;
}

unsigned முழு ext2fs_bitcount(மாறிலி வெறுமை *addr, unsigned முழு nbytes)
{
	மாறிலி unsigned எழுத்து *cp = addr;
	மாறிலி __u32 *p;
	unsigned முழு res = 0;

	போது (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (மாறிலி __u32 *) cp;

	போது (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (மாறிலி unsigned எழுத்து *) p;

	போது (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	திரும்பு res;
}
