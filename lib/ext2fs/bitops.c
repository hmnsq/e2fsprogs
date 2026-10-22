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
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

ಪೂರ್ಣಾಂಕ ext2fs_ಗಣ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr,ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ		ಮುಖವಾಡ, retval;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	retval = ಮುಖವಾಡ & *ADDR;
	*ADDR |= ಮುಖವಾಡ;
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_clear_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ		ಮುಖವಾಡ, retval;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	retval = ಮುಖವಾಡ & *ADDR;
	*ADDR &= ~ಮುಖವಾಡ;
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_test_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ			ಮುಖವಾಡ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	ಹಿಂತಿರುಗಿ (ಮುಖವಾಡ & *ADDR);
}

ಶೂನ್ಯ ext2fs_warn_bitmap(errcode_t errcode, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ arg,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
	ಆದರೆ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	ಇಲ್ಲದಿದ್ದರೆ
		com_err(0, errcode, "#%lu", arg);
#ಆದರೆ_ಅಂತ್ಯ
}

/* Bitmap functions that take a 64-bit offset */

ಪೂರ್ಣಾಂಕ ext2fs_ಗಣ_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ		ಮುಖವಾಡ, retval;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	retval = ಮುಖವಾಡ & *ADDR;
	*ADDR |= ಮುಖವಾಡ;
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_clear_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ		ಮುಖವಾಡ, retval;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	retval = ಮುಖವಾಡ & *ADDR;
	*ADDR &= ~ಮುಖವಾಡ;
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_test_bit64(__u64 nr, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * addr)
{
	ಪೂರ್ಣಾಂಕ			ಮುಖವಾಡ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	ಮುಖವಾಡ = 1 << (nr & 0x07);
	ಹಿಂತಿರುಗಿ (ಮುಖವಾಡ & *ADDR);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ popcount8(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ w)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	ಹಿಂತಿರುಗಿ (res + (res >> 4)) & 0x0F;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ popcount32(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ w)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	ಹಿಂತಿರುಗಿ (res + (res >> 16)) & 0x000000FF;
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_bitcount(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *addr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nbytes)
{
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *cp = addr;
	ಸ್ಥಿರಾಂಕ __u32 *p;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ res = 0;

	ಆಗ (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (ಸ್ಥಿರಾಂಕ __u32 *) cp;

	ಆಗ (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) p;

	ಆಗ (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	ಹಿಂತಿರುಗಿ res;
}
