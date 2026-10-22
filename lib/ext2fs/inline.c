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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#اگر HAVE_MALLOC_H
#شامل <malloc.h>
#endif

#شامل "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#شامل "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, تهی *ptr)
{
	errcode_t retval = 0;
	تهی **p = ptr;

	اگر (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	اگر (retval == ENOMEM)
		بازگشت EXT2_ET_NO_MEMORY;
#وگرنه  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	اگر (*p == پوچ) {
		اگر (errno)
			بازگشت errno;
		وگرنه
			بازگشت EXT2_ET_NO_MEMORY;
	}
#وگرنه  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	اگر (align > اندازه(long long))
		*p = valloc(size);
	وگرنه
#endif
		*p = malloc(size);
	اگر ((uintptr_t) *p & (align - 1)) {
		آزاد(*p);
		*p = 0;
	}
	اگر (*p == 0)
		بازگشت EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	بازگشت retval;
}

#ifdef DEBUG
ایستا صحیح isaligned(تهی *ptr, unsigned long align)
{
	بازگشت (((unsigned long) ptr & (align - 1)) == 0);
}

ایستا errcode_t test_memalign(unsigned long align)
{
	تهی *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	اگر (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	آزاد(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	بازگشت retval;
}

صحیح اصلی(صحیح argc, نویسه **argv)
{
	صحیح err = 0;

	اگر (test_memalign(4))
		err++;
	اگر (test_memalign(32))
		err++;
	اگر (test_memalign(1024))
		err++;
	اگر (test_memalign(4096))
		err++;
	بازگشت err;
}
#endif
