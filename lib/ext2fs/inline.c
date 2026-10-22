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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#𐡄𐡍 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#𐡄𐡍 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#𐡄𐡍 HAVE_MALLOC_H
#ضمن <malloc.h>
#endif

#ضمن "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#ضمن "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, فراغ *ptr)
{
	errcode_t retval = 0;
	فراغ **p = ptr;

	𐡄𐡍 (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	𐡄𐡍 (retval == ENOMEM)
		أرجع EXT2_ET_NO_MEMORY;
#وإلا  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	𐡄𐡍 (*p == عدم) {
		𐡄𐡍 (errno)
			أرجع errno;
		وإلا
			أرجع EXT2_ET_NO_MEMORY;
	}
#وإلا  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	𐡄𐡍 (align > حجم(long long))
		*p = valloc(size);
	وإلا
#endif
		*p = malloc(size);
	𐡄𐡍 ((uintptr_t) *p & (align - 1)) {
		حرر(*p);
		*p = 0;
	}
	𐡄𐡍 (*p == 0)
		أرجع EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	أرجع retval;
}

#ifdef DEBUG
ساكن صحيح isaligned(فراغ *ptr, unsigned long align)
{
	أرجع (((unsigned long) ptr & (align - 1)) == 0);
}

ساكن errcode_t test_memalign(unsigned long align)
{
	فراغ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	𐡄𐡍 (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	حرر(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	أرجع retval;
}

صحيح main(صحيح argc, حرف **argv)
{
	صحيح err = 0;

	𐡄𐡍 (test_memalign(4))
		err++;
	𐡄𐡍 (test_memalign(32))
		err++;
	𐡄𐡍 (test_memalign(1024))
		err++;
	𐡄𐡍 (test_memalign(4096))
		err++;
	أرجع err;
}
#endif
