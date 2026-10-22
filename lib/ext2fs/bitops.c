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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * C language bitmap functions written by Theodore Ts'o, 9/26/92.
 * Modified by Pete A. Zaitcev 7/14/95 to be portable to big endian
 * systems, as well as non-32 bit systems.
 */

పూర్ణాంకం ext2fs_సమితి_బిట్టు(గుర్తులేని పూర్ణాంకం nr,శూన్య * addr)
{
	పూర్ణాంకం		ముసుగు, retval;
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	retval = ముసుగు & *ADDR;
	*ADDR |= ముసుగు;
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_clear_బిట్టు(గుర్తులేని పూర్ణాంకం nr, శూన్య * addr)
{
	పూర్ణాంకం		ముసుగు, retval;
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	retval = ముసుగు & *ADDR;
	*ADDR &= ~ముసుగు;
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_test_బిట్టు(గుర్తులేని పూర్ణాంకం nr, స్థిరరాశి శూన్య * addr)
{
	పూర్ణాంకం			ముసుగు;
	స్థిరరాశి గుర్తులేని అక్షరం	*ADDR = (స్థిరరాశి గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	తిరిగి (ముసుగు & *ADDR);
}

శూన్య ext2fs_warn_bitmap(errcode_t errcode, గుర్తులేని దీర్ఘ arg,
			స్థిరరాశి అక్షరం *description)
{
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
	ఒకవేళ (description)
		com_err(0, errcode, "#%lu for %s", arg, description);
	లేకపోతే
		com_err(0, errcode, "#%lu", arg);
#అయితే_ముగింపు
}

/* Bitmap functions that take a 64-bit offset */

పూర్ణాంకం ext2fs_సమితి_bit64(__u64 nr, శూన్య * addr)
{
	పూర్ణాంకం		ముసుగు, retval;
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	retval = ముసుగు & *ADDR;
	*ADDR |= ముసుగు;
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_clear_bit64(__u64 nr, శూన్య * addr)
{
	పూర్ణాంకం		ముసుగు, retval;
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	retval = ముసుగు & *ADDR;
	*ADDR &= ~ముసుగు;
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_test_bit64(__u64 nr, స్థిరరాశి శూన్య * addr)
{
	పూర్ణాంకం			ముసుగు;
	స్థిరరాశి గుర్తులేని అక్షరం	*ADDR = (స్థిరరాశి గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	ముసుగు = 1 << (nr & 0x07);
	తిరిగి (ముసుగు & *ADDR);
}

స్థిర గుర్తులేని పూర్ణాంకం popcount8(గుర్తులేని పూర్ణాంకం w)
{
	గుర్తులేని పూర్ణాంకం res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	తిరిగి (res + (res >> 4)) & 0x0F;
}

స్థిర గుర్తులేని పూర్ణాంకం popcount32(గుర్తులేని పూర్ణాంకం w)
{
	గుర్తులేని పూర్ణాంకం res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	తిరిగి (res + (res >> 16)) & 0x000000FF;
}

గుర్తులేని పూర్ణాంకం ext2fs_bitcount(స్థిరరాశి శూన్య *addr, గుర్తులేని పూర్ణాంకం nbytes)
{
	స్థిరరాశి గుర్తులేని అక్షరం *cp = addr;
	స్థిరరాశి __u32 *p;
	గుర్తులేని పూర్ణాంకం res = 0;

	అయితే (((((uintptr_t) cp) & 3) != 0) && (nbytes > 0)) {
		res += popcount8(*cp++);
		nbytes--;
	}
	p = (స్థిరరాశి __u32 *) cp;

	అయితే (nbytes > 4) {
		res += popcount32(*p++);
		nbytes -= 4;
	}
	cp = (స్థిరరాశి గుర్తులేని అక్షరం *) p;

	అయితే (nbytes > 0) {
		res += popcount8(*cp++);
		nbytes--;
	}
	తిరిగి res;
}
