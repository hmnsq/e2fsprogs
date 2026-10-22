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
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ប្រសិនបើ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ប្រសិនបើ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ប្រសិនបើ HAVE_MALLOC_H
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

	ប្រសិនបើ (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	ប្រសិនបើ (retval == ENOMEM)
		ត្រឡប់ EXT2_ET_NO_MEMORY;
#បើមិនដូច្នេះ  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	ប្រសិនបើ (*p == NULL) {
		ប្រសិនបើ (errno)
			ត្រឡប់ errno;
		បើមិនដូច្នេះ
			ត្រឡប់ EXT2_ET_NO_MEMORY;
	}
#បើមិនដូច្នេះ  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	ប្រសិនបើ (align > parimaa_na(long long))
		*p = valloc(size);
	បើមិនដូច្នេះ
#endif
		*p = malloc(size);
	ប្រសិនបើ ((uintptr_t) *p & (align - 1)) {
		free(*p);
		*p = 0;
	}
	ប្រសិនបើ (*p == 0)
		ត្រឡប់ EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	ត្រឡប់ retval;
}

#ifdef DEBUG
static ចំនួនគត់ isaligned(void *ptr, unsigned long align)
{
	ត្រឡប់ (((unsigned long) ptr & (align - 1)) == 0);
}

static errcode_t test_memalign(unsigned long align)
{
	void *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ប្រសិនបើ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	free(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	ត្រឡប់ retval;
}

ចំនួនគត់ main(ចំនួនគត់ argc, var_na **argv)
{
	ចំនួនគត់ err = 0;

	ប្រសិនបើ (test_memalign(4))
		err++;
	ប្រសិនបើ (test_memalign(32))
		err++;
	ប្រសិនបើ (test_memalign(1024))
		err++;
	ប្រសិនបើ (test_memalign(4096))
		err++;
	ត្រឡប់ err;
}
#endif
