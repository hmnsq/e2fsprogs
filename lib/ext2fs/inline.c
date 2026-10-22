/*
 * inline.c --- Includes the inlined functions defined in the header
 * 	files as standalone functions, in case the application program
 * 	is compiled with inlining turned off.
 *
 * Copyright (C) 1993, 1994 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#నిర్వచితం_కాకపోతే XOPEN_ఆధారం
#నిర్వచించు XOPEN_ఆధారం 600	/* for posix_memalign() */
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_MALLOC_H
#అన్తర్భావ <malloc.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#నిర్వచించు INCLUDE_INLINE_FUNCS
#అన్తర్భావ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(గుర్తులేని దీర్ఘ కొలత,
			      గుర్తులేని దీర్ఘ align, శూన్య *ptr)
{
	errcode_t retval = 0;
	శూన్య **p = ptr;

	ఒకవేళ (align < 8)
		align = 8;
#నిర్వచితమైతే HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, కొలత);
	ఒకవేళ (retval == ENOMEM)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
#లేకపోతే  /* !HAVE_POSIX_MEMALIGN */
#నిర్వచితమైతే HAVE_MEMALIGN
	*p = memalign(align, కొలత);
	ఒకవేళ (*p == ఖాళీ) {
		ఒకవేళ (errno)
			తిరిగి errno;
		లేకపోతే
			తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	}
#లేకపోతే  /* !HAVE_MEMALIGN */
#నిర్వచితమైతే HAVE_VALLOC
	ఒకవేళ (align > పరిమాణం(దీర్ఘ దీర్ఘ))
		*p = valloc(కొలత);
	లేకపోతే
#అయితే_ముగింపు
		*p = జ్ఞాపకం_ఇవ్వు(కొలత);
	ఒకవేళ ((uintptr_t) *p & (align - 1)) {
		విడుదల(*p);
		*p = 0;
	}
	ఒకవేళ (*p == 0)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
#అయితే_ముగింపు	/* HAVE_MEMALIGN */
#అయితే_ముగింపు	/* HAVE_POSIX_MEMALIGN */
	తిరిగి retval;
}

#నిర్వచితమైతే DEBUG
స్థిర పూర్ణాంకం isaligned(శూన్య *ptr, గుర్తులేని దీర్ఘ align)
{
	తిరిగి (((గుర్తులేని దీర్ఘ) ptr & (align - 1)) == 0);
}

స్థిర errcode_t test_memalign(గుర్తులేని దీర్ఘ align)
{
	శూన్య *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ఒకవేళ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	విడుదల(ptr);
	రాయి("tst_memalign(%lu) is %s\n", align,
	       retval ? error_సందేశం(retval) : "OK");
	తిరిగి retval;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం err = 0;

	ఒకవేళ (test_memalign(4))
		err++;
	ఒకవేళ (test_memalign(32))
		err++;
	ఒకవేళ (test_memalign(1024))
		err++;
	ఒకవేళ (test_memalign(4096))
		err++;
	తిరిగి err;
}
#అయితే_ముగింపు
