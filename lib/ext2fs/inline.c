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
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#නම් HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#නම් HAVE_MALLOC_H
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

	නම් (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	නම් (retval == ENOMEM)
		ආපසු EXT2_ET_NO_MEMORY;
#නැතහොත්  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	නම් (*p == NULL) {
		නම් (errno)
			ආපසු errno;
		නැතහොත්
			ආපසු EXT2_ET_NO_MEMORY;
	}
#නැතහොත්  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	නම් (align > parimaa_na(long long))
		*p = valloc(size);
	නැතහොත්
#endif
		*p = malloc(size);
	නම් ((uintptr_t) *p & (align - 1)) {
		free(*p);
		*p = 0;
	}
	නම් (*p == 0)
		ආපසු EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	ආපසු retval;
}

#ifdef DEBUG
static පූර්ණ_සංඛ්‍යා isaligned(void *ptr, unsigned long align)
{
	ආපසු (((unsigned long) ptr & (align - 1)) == 0);
}

static errcode_t test_memalign(unsigned long align)
{
	void *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	නම් (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	free(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	ආපසු retval;
}

පූර්ණ_සංඛ්‍යා main(පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
	පූර්ණ_සංඛ්‍යා err = 0;

	නම් (test_memalign(4))
		err++;
	නම් (test_memalign(32))
		err++;
	නම් (test_memalign(1024))
		err++;
	නම් (test_memalign(4096))
		err++;
	ආපසු err;
}
#endif
