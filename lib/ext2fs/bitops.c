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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ਪੂਰਨ ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr,ਖਾਲੀ * addr)
{
	ਪੂਰਨ		ਨਕਾਬ, retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	retval = ਨਕਾਬ & *ADDR;
	*ADDR |= ਨਕਾਬ;
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਖਾਲੀ * addr)
{
	ਪੂਰਨ		ਨਕਾਬ, retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	retval = ਨਕਾਬ & *ADDR;
	*ADDR &= ~ਨਕਾਬ;
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਅਚਲ ਖਾਲੀ * addr)
{
	ਪੂਰਨ			ਨਕਾਬ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	ਵਾਪਸ (ਨਕਾਬ & *ADDR);
}

ਖਾਲੀ ext2fs_warn_bitmap(errcode_t errcode, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ arg,
			ਅਚਲ ਅੱਖਰ *description)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
	ਜੇ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ਨਹੀਂਤਾਂ
		com_err(0, errcode, "#%lu", arg);
#ਜੇ_ਅੰਤ
}

/* Bitmap functions that take a 64-bit offset */

ਪੂਰਨ ext2fs_ਸੈੱਟ_bit64(__u64 nr, ਖਾਲੀ * addr)
{
	ਪੂਰਨ		ਨਕਾਬ, retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	retval = ਨਕਾਬ & *ADDR;
	*ADDR |= ਨਕਾਬ;
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_clear_bit64(__u64 nr, ਖਾਲੀ * addr)
{
	ਪੂਰਨ		ਨਕਾਬ, retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	retval = ਨਕਾਬ & *ADDR;
	*ADDR &= ~ਨਕਾਬ;
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_test_bit64(__u64 nr, ਅਚਲ ਖਾਲੀ * addr)
{
	ਪੂਰਨ			ਨਕਾਬ;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	ਨਕਾਬ = 1 << (nr & 0x07);
	ਵਾਪਸ (ਨਕਾਬ & *ADDR);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ popcount8(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ w)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ਵਾਪਸ (res + (res >> 4)) & 0x0F;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ popcount32(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ w)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ਵਾਪਸ (res + (res >> 16)) & 0x000000FF;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_bitcount(ਅਚਲ ਖਾਲੀ *addr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nbytes)
{
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp = addr;
	ਅਚਲ __u32 *p;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ res = 0;

	ਜਦੋਂ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ਅਚਲ __u32 *) cp;

	ਜਦੋਂ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) p;

	ਜਦੋਂ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ਵਾਪਸ res;
}
