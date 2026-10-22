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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_MALLOC_H
#ଅନ୍ତର୍ଭାଵ <malloc.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, ଶୂନ୍ୟ *ptr)
{
	errcode_t retval = 0;
	ଶୂନ୍ୟ **p = ptr;

	ଯଦି (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	ଯଦି (retval == ENOMEM)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
#ଅନ୍ୟଥା  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	ଯଦି (*p == ଖାଲି) {
		ଯଦି (errno)
			ଫେରନ୍ତୁ errno;
		ଅନ୍ୟଥା
			ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	}
#ଅନ୍ୟଥା  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	ଯଦି (align > ଆକାର(long long))
		*p = valloc(size);
	ଅନ୍ୟଥା
#endif
		*p = malloc(size);
	ଯଦି ((uintptr_t) *p & (align - 1)) {
		ମୁକ୍ତ(*p);
		*p = 0;
	}
	ଯଦି (*p == 0)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	ଫେରନ୍ତୁ retval;
}

#ifdef DEBUG
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା isaligned(ଶୂନ୍ୟ *ptr, unsigned long align)
{
	ଫେରନ୍ତୁ (((unsigned long) ptr & (align - 1)) == 0);
}

ସ୍ଥିର errcode_t test_memalign(unsigned long align)
{
	ଶୂନ୍ୟ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ଯଦି (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	ମୁକ୍ତ(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ଯଦି (test_memalign(4))
		err++;
	ଯଦି (test_memalign(32))
		err++;
	ଯଦି (test_memalign(1024))
		err++;
	ଯଦି (test_memalign(4096))
		err++;
	ଫେରନ୍ତୁ err;
}
#endif
