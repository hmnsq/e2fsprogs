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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ಪೂರ್ಣಾಙ್ಕ ext2fs_set_bit(unsigned ಪೂರ್ಣಾಙ್ಕ nr,ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ		mask, retval;
	unsigned ವರ್ಣ	*ADDR = (unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_clear_bit(unsigned ಪೂರ್ಣಾಙ್ಕ nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ		mask, retval;
	unsigned ವರ್ಣ	*ADDR = (unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_test_bit(unsigned ಪೂರ್ಣಾಙ್ಕ nr, ಧ್ರುವ ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ			mask;
	ಧ್ರುವ unsigned ವರ್ಣ	*ADDR = (ಧ್ರುವ unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ಪ್ರತ್ಯಾಗಮ (mask & *ADDR);
}

ಶೂನ್ಯ ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ಧ್ರುವ ವರ್ಣ *description)
{
#ifndef OMIT_COM_ERR
	ಯದಿ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ಅನ್ಯಥಾ
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

ಪೂರ್ಣಾಙ್ಕ ext2fs_set_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ		mask, retval;
	unsigned ವರ್ಣ	*ADDR = (unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_clear_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ		mask, retval;
	unsigned ವರ್ಣ	*ADDR = (unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_test_bit64(__u64 nr, ಧ್ರುವ ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಙ್ಕ			mask;
	ಧ್ರುವ unsigned ವರ್ಣ	*ADDR = (ಧ್ರುವ unsigned ವರ್ಣ *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	ಪ್ರತ್ಯಾಗಮ (mask & *ADDR);
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ popcount8(unsigned ಪೂರ್ಣಾಙ್ಕ w)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ಪ್ರತ್ಯಾಗಮ (res + (res >> 4)) & 0x0F;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ popcount32(unsigned ಪೂರ್ಣಾಙ್ಕ w)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ಪ್ರತ್ಯಾಗಮ (res + (res >> 16)) & 0x000000FF;
}

unsigned ಪೂರ್ಣಾಙ್ಕ ext2fs_bitcount(ಧ್ರುವ ಶೂನ್ಯ *addr, unsigned ಪೂರ್ಣಾಙ್ಕ nbytes)
{
	ಧ್ರುವ unsigned ವರ್ಣ *cp = addr;
	ಧ್ರುವ __u32 *p;
	unsigned ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯಾವತ್ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ಧ್ರುವ __u32 *) cp;

	ಯಾವತ್ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ಧ್ರುವ unsigned ವರ್ಣ *) p;

	ಯಾವತ್ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ಪ್ರತ್ಯಾಗಮ res;
}
