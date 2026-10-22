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

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#nếu HAVE_MALLOC_H
#include <malloc.h>
#endif

#include "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#include "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, void *ptr)
{
	errcode_t retval = 0;
	void **p = ptr;

	nếu (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	nếu (retval == ENOMEM)
		trả_về EXT2_ET_NO_MEMORY;
#không_thì  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	nếu (*p == NULL) {
		nếu (errno)
			trả_về errno;
		không_thì
			trả_về EXT2_ET_NO_MEMORY;
	}
#không_thì  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	nếu (align > sizeof(long long))
		*p = valloc(size);
	không_thì
#endif
		*p = malloc(size);
	nếu ((uintptr_t) *p & (align - 1)) {
		free(*p);
		*p = 0;
	}
	nếu (*p == 0)
		trả_về EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	trả_về retval;
}

#ifdef DEBUG
static số_nguyên isaligned(void *ptr, unsigned long align)
{
	trả_về (((unsigned long) ptr & (align - 1)) == 0);
}

static errcode_t test_memalign(unsigned long align)
{
	void *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	nếu (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	free(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	trả_về retval;
}

số_nguyên main(số_nguyên argc, char **argv)
{
	số_nguyên err = 0;

	nếu (test_memalign(4))
		err++;
	nếu (test_memalign(32))
		err++;
	nếu (test_memalign(1024))
		err++;
	nếu (test_memalign(4096))
		err++;
	trả_về err;
}
#endif
