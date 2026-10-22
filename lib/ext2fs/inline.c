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

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600	/* for posix_memalign() */
#endif

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#যদি HAVE_MALLOC_H
#অন্তর্ভাৱ <malloc.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#অন্তর্ভাৱ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, শূন্য *ptr)
{
	errcode_t retval = 0;
	শূন্য **p = ptr;

	যদি (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	যদি (retval == ENOMEM)
		ঘূৰাই EXT2_ET_NO_MEMORY;
#অন্যথা  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	যদি (*p == রিক্ত) {
		যদি (errno)
			ঘূৰাই errno;
		অন্যথা
			ঘূৰাই EXT2_ET_NO_MEMORY;
	}
#অন্যথা  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	যদি (align > পরিমাণ(long long))
		*p = valloc(size);
	অন্যথা
#endif
		*p = malloc(size);
	যদি ((uintptr_t) *p & (align - 1)) {
		মুক্ত(*p);
		*p = 0;
	}
	যদি (*p == 0)
		ঘূৰাই EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	ঘূৰাই retval;
}

#ifdef DEBUG
স্থির পূর্ণাঙ্ক isaligned(শূন্য *ptr, unsigned long align)
{
	ঘূৰাই (((unsigned long) ptr & (align - 1)) == 0);
}

স্থির errcode_t test_memalign(unsigned long align)
{
	শূন্য *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	যদি (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	মুক্ত(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	ঘূৰাই retval;
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
	পূর্ণাঙ্ক err = 0;

	যদি (test_memalign(4))
		err++;
	যদি (test_memalign(32))
		err++;
	যদি (test_memalign(1024))
		err++;
	যদি (test_memalign(4096))
		err++;
	ঘূৰাই err;
}
#endif
