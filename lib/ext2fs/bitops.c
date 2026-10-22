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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

പൂർണ്ണസംഖ്യ ext2fs_set_bit(unsigned പൂർണ്ണസംഖ്യ nr,ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ		mask, retval;
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_clear_bit(unsigned പൂർണ്ണസംഖ്യ nr, ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ		mask, retval;
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_test_bit(unsigned പൂർണ്ണസംഖ്യ nr, സ്ഥിരം ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ			mask;
	സ്ഥിരം unsigned അക്ഷരം	*ADDR = (സ്ഥിരം unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	തിരികെ (mask & *ADDR);
}

ശൂന്യം ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			സ്ഥിരം അക്ഷരം *description)
{
#ifndef OMIT_COM_ERR
	എങ്കിൽ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	അല്ലെങ്കിൽ
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

പൂർണ്ണസംഖ്യ ext2fs_set_bit64(__u64 nr, ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ		mask, retval;
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_clear_bit64(__u64 nr, ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ		mask, retval;
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_test_bit64(__u64 nr, സ്ഥിരം ശൂന്യം * addr)
{
	പൂർണ്ണസംഖ്യ			mask;
	സ്ഥിരം unsigned അക്ഷരം	*ADDR = (സ്ഥിരം unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	തിരികെ (mask & *ADDR);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ popcount8(unsigned പൂർണ്ണസംഖ്യ w)
{
	unsigned പൂർണ്ണസംഖ്യ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	തിരികെ (res + (res >> 4)) & 0x0F;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ popcount32(unsigned പൂർണ്ണസംഖ്യ w)
{
	unsigned പൂർണ്ണസംഖ്യ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	തിരികെ (res + (res >> 16)) & 0x000000FF;
}

unsigned പൂർണ്ണസംഖ്യ ext2fs_bitcount(സ്ഥിരം ശൂന്യം *addr, unsigned പൂർണ്ണസംഖ്യ nbytes)
{
	സ്ഥിരം unsigned അക്ഷരം *cp = addr;
	സ്ഥിരം __u32 *p;
	unsigned പൂർണ്ണസംഖ്യ res = 0;

	ആകുമ്പോൾ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (സ്ഥിരം __u32 *) cp;

	ആകുമ്പോൾ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (സ്ഥിരം unsigned അക്ഷരം *) p;

	ആകുമ്പോൾ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	തിരികെ res;
}
