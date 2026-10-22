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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

पूर्णाङ्क ext2fs_set_bit(unsigned पूर्णाङ्क nr,शून्य * addr)
{
	पूर्णाङ्क		mask, retval;
	unsigned वर्ण	*ADDR = (unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	पटिनिवत्त retval;
}

पूर्णाङ्क ext2fs_clear_bit(unsigned पूर्णाङ्क nr, शून्य * addr)
{
	पूर्णाङ्क		mask, retval;
	unsigned वर्ण	*ADDR = (unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	पटिनिवत्त retval;
}

पूर्णाङ्क ext2fs_test_bit(unsigned पूर्णाङ्क nr, ध्रुव शून्य * addr)
{
	पूर्णाङ्क			mask;
	ध्रुव unsigned वर्ण	*ADDR = (ध्रुव unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	पटिनिवत्त (mask & *ADDR);
}

शून्य ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			ध्रुव वर्ण *description)
{
#ifndef OMIT_COM_ERR
	सचे (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	अञ्ञथा
		com_err(0, errcode, "#%lu", arg);
#endif
}

/* Bitmap functions that take a 64-bit offset */

पूर्णाङ्क ext2fs_set_bit64(__u64 nr, शून्य * addr)
{
	पूर्णाङ्क		mask, retval;
	unsigned वर्ण	*ADDR = (unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	पटिनिवत्त retval;
}

पूर्णाङ्क ext2fs_clear_bit64(__u64 nr, शून्य * addr)
{
	पूर्णाङ्क		mask, retval;
	unsigned वर्ण	*ADDR = (unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	पटिनिवत्त retval;
}

पूर्णाङ्क ext2fs_test_bit64(__u64 nr, ध्रुव शून्य * addr)
{
	पूर्णाङ्क			mask;
	ध्रुव unsigned वर्ण	*ADDR = (ध्रुव unsigned वर्ण *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	पटिनिवत्त (mask & *ADDR);
}

स्थिर unsigned पूर्णाङ्क popcount8(unsigned पूर्णाङ्क w)
{
	unsigned पूर्णाङ्क res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	पटिनिवत्त (res + (res >> 4)) & 0x0F;
}

स्थिर unsigned पूर्णाङ्क popcount32(unsigned पूर्णाङ्क w)
{
	unsigned पूर्णाङ्क res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	पटिनिवत्त (res + (res >> 16)) & 0x000000FF;
}

unsigned पूर्णाङ्क ext2fs_bitcount(ध्रुव शून्य *addr, unsigned पूर्णाङ्क nbytes)
{
	ध्रुव unsigned वर्ण *cp = addr;
	ध्रुव __u32 *p;
	unsigned पूर्णाङ्क res = 0;

	याव (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ध्रुव __u32 *) cp;

	याव (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ध्रुव unsigned वर्ण *) p;

	याव (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	पटिनिवत्त res;
}
