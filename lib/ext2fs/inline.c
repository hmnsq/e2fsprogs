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

#اگر_تعریف_نشده XOPEN_منبع
#تعریف_کن XOPEN_منبع 600	/* for posix_memalign() */
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#إذا HAVE_MALLOC_H
#ضمن <malloc.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#تعریف_کن INCLUDE_INLINE_FUNCS
#ضمن "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(بیعلامت دراز اندازه,
			      بیعلامت دراز align, فراغ *ptr)
{
	errcode_t retval = 0;
	فراغ **p = ptr;

	إذا (align < 8)
		align = 8;
#اگر_تعریف_شده HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, اندازه);
	إذا (retval == ENOMEM)
		أرجع EXT2_ET_NO_حافظه;
#وإلا  /* !HAVE_POSIX_MEMALIGN */
#اگر_تعریف_شده HAVE_MEMALIGN
	*p = memalign(align, اندازه);
	إذا (*p == عدم) {
		إذا (errno)
			أرجع errno;
		وإلا
			أرجع EXT2_ET_NO_حافظه;
	}
#وإلا  /* !HAVE_MEMALIGN */
#اگر_تعریف_شده HAVE_VALLOC
	إذا (align > حجم(دراز دراز))
		*p = valloc(اندازه);
	وإلا
#پایان_اگر
		*p = حافظه_بده(اندازه);
	إذا ((uintptr_t) *p & (align - 1)) {
		حرر(*p);
		*p = 0;
	}
	إذا (*p == 0)
		أرجع EXT2_ET_NO_حافظه;
#پایان_اگر	/* HAVE_MEMALIGN */
#پایان_اگر	/* HAVE_POSIX_MEMALIGN */
	أرجع retval;
}

#اگر_تعریف_شده DEBUG
ساكن صحيح isaligned(فراغ *ptr, بیعلامت دراز align)
{
	أرجع (((بیعلامت دراز) ptr & (align - 1)) == 0);
}

ساكن errcode_t test_memalign(بیعلامت دراز align)
{
	فراغ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	إذا (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	حرر(ptr);
	چاپ_کن("tst_memalign(%lu) is %s\n", align,
	       retval ? error_پیام(retval) : "OK");
	أرجع retval;
}

صحيح اصلی(صحيح argc, حرف **argv)
{
	صحيح err = 0;

	إذا (test_memalign(4))
		err++;
	إذا (test_memalign(32))
		err++;
	إذا (test_memalign(1024))
		err++;
	إذا (test_memalign(4096))
		err++;
	أرجع err;
}
#پایان_اگر
