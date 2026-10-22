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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_MALLOC_H
#અન્તર્ભાવ <malloc.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#અન્તર્ભાવ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, શૂન્ય *ptr)
{
	errcode_t retval = 0;
	શૂન્ય **p = ptr;

	જો (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	જો (retval == ENOMEM)
		પાછા EXT2_ET_NO_MEMORY;
#નહીંતર  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	જો (*p == ખાલી) {
		જો (errno)
			પાછા errno;
		નહીંતર
			પાછા EXT2_ET_NO_MEMORY;
	}
#નહીંતર  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	જો (align > કદ(long long))
		*p = valloc(size);
	નહીંતર
#endif
		*p = malloc(size);
	જો ((uintptr_t) *p & (align - 1)) {
		મુક્ત(*p);
		*p = 0;
	}
	જો (*p == 0)
		પાછા EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	પાછા retval;
}

#ifdef DEBUG
સ્થિર પૂર્ણાંક isaligned(શૂન્ય *ptr, unsigned long align)
{
	પાછા (((unsigned long) ptr & (align - 1)) == 0);
}

સ્થિર errcode_t test_memalign(unsigned long align)
{
	શૂન્ય *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	જો (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	મુક્ત(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	પાછા retval;
}

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	પૂર્ણાંક err = 0;

	જો (test_memalign(4))
		err++;
	જો (test_memalign(32))
		err++;
	જો (test_memalign(1024))
		err++;
	જો (test_memalign(4096))
		err++;
	પાછા err;
}
#endif
