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
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

صحیح ext2fs_سیٹ_ثنائی_ہندسہ(غیرعلامتی صحیح nr,خالی * addr)
{
	صحیح		نقاب, retval;
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	retval = نقاب & *ADDR;
	*ADDR |= نقاب;
	واپس retval;
}

صحیح ext2fs_clear_ثنائی_ہندسہ(غیرعلامتی صحیح nr, خالی * addr)
{
	صحیح		نقاب, retval;
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	retval = نقاب & *ADDR;
	*ADDR &= ~نقاب;
	واپس retval;
}

صحیح ext2fs_test_ثنائی_ہندسہ(غیرعلامتی صحیح nr, ثابت خالی * addr)
{
	صحیح			نقاب;
	ثابت غیرعلامتی حرف	*ADDR = (ثابت غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	واپس (نقاب & *ADDR);
}

خالی ext2fs_warn_bitmap(errcode_t errcode, غیرعلامتی طویل arg,
			ثابت حرف *description)
{
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
	اگر (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ورنہ
		com_err(0, errcode, "#%lu", arg);
#اگر_ختم
}

/* Bitmap functions that take a 64-bit offset */

صحیح ext2fs_سیٹ_bit64(__u64 nr, خالی * addr)
{
	صحیح		نقاب, retval;
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	retval = نقاب & *ADDR;
	*ADDR |= نقاب;
	واپس retval;
}

صحیح ext2fs_clear_bit64(__u64 nr, خالی * addr)
{
	صحیح		نقاب, retval;
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	retval = نقاب & *ADDR;
	*ADDR &= ~نقاب;
	واپس retval;
}

صحیح ext2fs_test_bit64(__u64 nr, ثابت خالی * addr)
{
	صحیح			نقاب;
	ثابت غیرعلامتی حرف	*ADDR = (ثابت غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	نقاب = 1 << (nr & 0x07);
	واپس (نقاب & *ADDR);
}

ساکن غیرعلامتی صحیح popcount8(غیرعلامتی صحیح w)
{
	غیرعلامتی صحیح res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	واپس (res + (res >> 4)) & 0x0F;
}

ساکن غیرعلامتی صحیح popcount32(غیرعلامتی صحیح w)
{
	غیرعلامتی صحیح res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	واپس (res + (res >> 16)) & 0x000000FF;
}

غیرعلامتی صحیح ext2fs_bitcount(ثابت خالی *addr, غیرعلامتی صحیح nbytes)
{
	ثابت غیرعلامتی حرف *cp = addr;
	ثابت __u32 *p;
	غیرعلامتی صحیح res = 0;

	جب (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ثابت __u32 *) cp;

	جب (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ثابت غیرعلامتی حرف *) p;

	جب (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	واپس res;
}
