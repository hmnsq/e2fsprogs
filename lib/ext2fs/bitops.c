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
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

පූර්ණ_සංඛ්‍යා ext2fs_set_bit(unsigned පූර්ණ_සංඛ්‍යා nr,void * addr)
{
	පූර්ණ_සංඛ්‍යා		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ආපසු retval;
}

පූර්ණ_සංඛ්‍යා ext2fs_clear_bit(unsigned පූර්ණ_සංඛ්‍යා nr, void * addr)
{
	පූර්ණ_සංඛ්‍යා		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ආපසු retval;
}

පූර්ණ_සංඛ්‍යා ext2fs_test_bit(unsigned පූර්ණ_සංඛ්‍යා nr, const void * addr)
{
	පූර්ණ_සංඛ්‍යා			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ආපසු (mask & *ADDR);
}

void ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			const var_na *description)
{
#ifndef OMIT_COM_ERR
	නම් (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	නැතහොත්
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

පූර්ණ_සංඛ්‍යා ext2fs_set_bit64(__u64 nr, void * addr)
{
	පූර්ණ_සංඛ්‍යා		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ආපසු retval;
}

පූර්ණ_සංඛ්‍යා ext2fs_clear_bit64(__u64 nr, void * addr)
{
	පූර්ණ_සංඛ්‍යා		mask, retval;
	unsigned var_na	*ADDR = (unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ආපසු retval;
}

පූර්ණ_සංඛ්‍යා ext2fs_test_bit64(__u64 nr, const void * addr)
{
	පූර්ණ_සංඛ්‍යා			mask;
	const unsigned var_na	*ADDR = (const unsigned var_na *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ආපසු (mask & *ADDR);
}

static unsigned පූර්ණ_සංඛ්‍යා popcount8(unsigned පූර්ණ_සංඛ්‍යා w)
{
	unsigned පූර්ණ_සංඛ්‍යා res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ආපසු (res + (res >> 4)) & 0x0F;
}

static unsigned පූර්ණ_සංඛ්‍යා popcount32(unsigned පූර්ණ_සංඛ්‍යා w)
{
	unsigned පූර්ණ_සංඛ්‍යා res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ආපසු (res + (res >> 16)) & 0x000000FF;
}

unsigned පූර්ණ_සංඛ්‍යා ext2fs_bitcount(const void *addr, unsigned පූර්ණ_සංඛ්‍යා nbytes)
{
	const unsigned var_na *cp = addr;
	const __u32 *p;
	unsigned පූර්ණ_සංඛ්‍යා res = 0;

	අතරතුර (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (const __u32 *) cp;

	අතරතුර (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (const unsigned var_na *) p;

	අතරතුර (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ආපසු res;
}
