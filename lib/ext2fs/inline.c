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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_MALLOC_H
#ಅನ್ತರ್ಭಾವ <malloc.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, ಶೂನ್ಯ *ptr)
{
	errcode_t retval = 0;
	ಶೂನ್ಯ **p = ptr;

	ಯದಿ (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	ಯದಿ (retval == ENOMEM)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
#ಅನ್ಯಥಾ  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	ಯದಿ (*p == ರಿಕ್ತ) {
		ಯದಿ (errno)
			ಪ್ರತ್ಯಾಗಮ errno;
		ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	}
#ಅನ್ಯಥಾ  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	ಯದಿ (align > ಪರಿಮಾಣ(long long))
		*p = valloc(size);
	ಅನ್ಯಥಾ
#endif
		*p = malloc(size);
	ಯದಿ ((uintptr_t) *p & (align - 1)) {
		ಮುಕ್ತ(*p);
		*p = 0;
	}
	ಯದಿ (*p == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	ಪ್ರತ್ಯಾಗಮ retval;
}

#ifdef DEBUG
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ isaligned(ಶೂನ್ಯ *ptr, unsigned long align)
{
	ಪ್ರತ್ಯಾಗಮ (((unsigned long) ptr & (align - 1)) == 0);
}

ಸ್ಥಿರ errcode_t test_memalign(unsigned long align)
{
	ಶೂನ್ಯ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ಯದಿ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	ಮುಕ್ತ(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ err = 0;

	ಯದಿ (test_memalign(4))
		err++;
	ಯದಿ (test_memalign(32))
		err++;
	ಯದಿ (test_memalign(1024))
		err++;
	ಯದಿ (test_memalign(4096))
		err++;
	ಪ್ರತ್ಯಾಗಮ err;
}
#endif
