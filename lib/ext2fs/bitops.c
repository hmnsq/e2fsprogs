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
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

पूर्णाङ्क ext2fs_समुच्चयः_bit(अचिह्नित पूर्णाङ्क nr,शून्य * addr)
{
	पूर्णाङ्क		आवरणम्, retval;
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	retval = आवरणम् & *ADDR;
	*ADDR |= आवरणम्;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_clear_bit(अचिह्नित पूर्णाङ्क nr, शून्य * addr)
{
	पूर्णाङ्क		आवरणम्, retval;
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	retval = आवरणम् & *ADDR;
	*ADDR &= ~आवरणम्;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_test_bit(अचिह्नित पूर्णाङ्क nr, ध्रुव शून्य * addr)
{
	पूर्णाङ्क			आवरणम्;
	ध्रुव अचिह्नित वर्ण	*ADDR = (ध्रुव अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	प्रत्यागम (आवरणम् & *ADDR);
}

शून्य ext2fs_warn_bitmap(errcode_t errcode, अचिह्नित दीर्घ arg,
			ध्रुव वर्ण *description)
{
#यदि_अपरिभाषितम् OMIT_COM_ERR
	यदि (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	अन्यथा
		com_err(0, errcode, "#%lu", arg);
#यदि_अन्तः
}

/* Bitmap functions that take a 64-bit offset */

पूर्णाङ्क ext2fs_समुच्चयः_bit64(__u64 nr, शून्य * addr)
{
	पूर्णाङ्क		आवरणम्, retval;
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	retval = आवरणम् & *ADDR;
	*ADDR |= आवरणम्;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_clear_bit64(__u64 nr, शून्य * addr)
{
	पूर्णाङ्क		आवरणम्, retval;
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	retval = आवरणम् & *ADDR;
	*ADDR &= ~आवरणम्;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_test_bit64(__u64 nr, ध्रुव शून्य * addr)
{
	पूर्णाङ्क			आवरणम्;
	ध्रुव अचिह्नित वर्ण	*ADDR = (ध्रुव अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	आवरणम् = 1 << (nr & 0x07);
	प्रत्यागम (आवरणम् & *ADDR);
}

स्थिर अचिह्नित पूर्णाङ्क popcount8(अचिह्नित पूर्णाङ्क w)
{
	अचिह्नित पूर्णाङ्क res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	प्रत्यागम (res + (res >> 4)) & 0x0F;
}

स्थिर अचिह्नित पूर्णाङ्क popcount32(अचिह्नित पूर्णाङ्क w)
{
	अचिह्नित पूर्णाङ्क res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	प्रत्यागम (res + (res >> 16)) & 0x000000FF;
}

अचिह्नित पूर्णाङ्क ext2fs_bitcount(ध्रुव शून्य *addr, अचिह्नित पूर्णाङ्क nbytes)
{
	ध्रुव अचिह्नित वर्ण *cp = addr;
	ध्रुव __u32 *p;
	अचिह्नित पूर्णाङ्क res = 0;

	यावत् (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ध्रुव __u32 *) cp;

	यावत् (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ध्रुव अचिह्नित वर्ण *) p;

	यावत् (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	प्रत्यागम res;
}
