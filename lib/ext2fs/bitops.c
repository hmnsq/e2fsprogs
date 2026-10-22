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
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

صحيح ext2fs_مجموعه_بیت(بیعلامت صحيح nr,فراغ * addr)
{
	صحيح		ماسک, retval;
	بیعلامت حرف	*ADDR = (بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	retval = ماسک & *ADDR;
	*ADDR |= ماسک;
	أرجع retval;
}

صحيح ext2fs_clear_بیت(بیعلامت صحيح nr, فراغ * addr)
{
	صحيح		ماسک, retval;
	بیعلامت حرف	*ADDR = (بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	retval = ماسک & *ADDR;
	*ADDR &= ~ماسک;
	أرجع retval;
}

صحيح ext2fs_test_بیت(بیعلامت صحيح nr, ثابت فراغ * addr)
{
	صحيح			ماسک;
	ثابت بیعلامت حرف	*ADDR = (ثابت بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	أرجع (ماسک & *ADDR);
}

فراغ ext2fs_warn_bitmap(errcode_t errcode, بیعلامت دراز arg,
			ثابت حرف *description)
{
#اگر_تعریف_نشده OMIT_COM_ERR
	إذا (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	وإلا
		com_err(0, errcode, "#%lu", arg);
#پایان_اگر
}

/* Bitmap functions that take a 64-bit offset */

صحيح ext2fs_مجموعه_bit64(__u64 nr, فراغ * addr)
{
	صحيح		ماسک, retval;
	بیعلامت حرف	*ADDR = (بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	retval = ماسک & *ADDR;
	*ADDR |= ماسک;
	أرجع retval;
}

صحيح ext2fs_clear_bit64(__u64 nr, فراغ * addr)
{
	صحيح		ماسک, retval;
	بیعلامت حرف	*ADDR = (بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	retval = ماسک & *ADDR;
	*ADDR &= ~ماسک;
	أرجع retval;
}

صحيح ext2fs_test_bit64(__u64 nr, ثابت فراغ * addr)
{
	صحيح			ماسک;
	ثابت بیعلامت حرف	*ADDR = (ثابت بیعلامت حرف *) addr;

	ADDR += nr >> 3;
	ماسک = 1 << (nr & 0x07);
	أرجع (ماسک & *ADDR);
}

ساكن بیعلامت صحيح popcount8(بیعلامت صحيح w)
{
	بیعلامت صحيح res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	أرجع (res + (res >> 4)) & 0x0F;
}

ساكن بیعلامت صحيح popcount32(بیعلامت صحيح w)
{
	بیعلامت صحيح res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	أرجع (res + (res >> 16)) & 0x000000FF;
}

بیعلامت صحيح ext2fs_bitcount(ثابت فراغ *addr, بیعلامت صحيح nbytes)
{
	ثابت بیعلامت حرف *cp = addr;
	ثابت __u32 *p;
	بیعلامت صحيح res = 0;

	طالما (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ثابت __u32 *) cp;

	طالما (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ثابت بیعلامت حرف *) p;

	طالما (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	أرجع res;
}
