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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

पूर्णांक ext2fs_set_bit(अचिन्हित पूर्णांक nr,व्योम * addr)
{
	पूर्णांक		mask, retval;
	अचिन्हित अक्षर	*ADDR = (अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	वापस retval;
}

पूर्णांक ext2fs_clear_bit(अचिन्हित पूर्णांक nr, व्योम * addr)
{
	पूर्णांक		mask, retval;
	अचिन्हित अक्षर	*ADDR = (अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	वापस retval;
}

पूर्णांक ext2fs_test_bit(अचिन्हित पूर्णांक nr, स्थिर व्योम * addr)
{
	पूर्णांक			mask;
	स्थिर अचिन्हित अक्षर	*ADDR = (स्थिर अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	वापस (mask & *ADDR);
}

व्योम ext2fs_warn_bitmap(errcode_t errcode, अचिन्हित दीर्घ arg,
			स्थिर अक्षर *description)
{
#अगर_अघोषित OMIT_COM_ERR
	अगर (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	अन्यथा
		com_err(0, errcode, "#%lu", arg);
#पूर्ण_अगर
}

/* Bitmap functions that take a 64-bit offset */

पूर्णांक ext2fs_set_bit64(__u64 nr, व्योम * addr)
{
	पूर्णांक		mask, retval;
	अचिन्हित अक्षर	*ADDR = (अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR |= mask;
	वापस retval;
}

पूर्णांक ext2fs_clear_bit64(__u64 nr, व्योम * addr)
{
	पूर्णांक		mask, retval;
	अचिन्हित अक्षर	*ADDR = (अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	retval = mask & *ADDR;
	*ADDR &= ~mask;
	वापस retval;
}

पूर्णांक ext2fs_test_bit64(__u64 nr, स्थिर व्योम * addr)
{
	पूर्णांक			mask;
	स्थिर अचिन्हित अक्षर	*ADDR = (स्थिर अचिन्हित अक्षर *) addr;

	ADDR += nr >> 3;
	mask = 1 << (nr & 0x07);
	वापस (mask & *ADDR);
}

अटल अचिन्हित पूर्णांक popcount8(अचिन्हित पूर्णांक w)
{
	अचिन्हित पूर्णांक res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	वापस (res + (res >> 4)) & 0x0F;
}

अटल अचिन्हित पूर्णांक popcount32(अचिन्हित पूर्णांक w)
{
	अचिन्हित पूर्णांक res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	वापस (res + (res >> 16)) & 0x000000FF;
}

अचिन्हित पूर्णांक ext2fs_bitcount(स्थिर व्योम *addr, अचिन्हित पूर्णांक nbytes)
{
	स्थिर अचिन्हित अक्षर *cp = addr;
	स्थिर __u32 *p;
	अचिन्हित पूर्णांक res = 0;

	जबतक (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (स्थिर __u32 *) cp;

	जबतक (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (स्थिर अचिन्हित अक्षर *) p;

	जबतक (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	वापस res;
}
