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

#که_نه_تعریف_شوی XOPEN_سرچینه
#تعریف_کړه XOPEN_سرچینه 600	/* for posix_memalign() */
#د_که_پای

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#إذا HAVE_MALLOC_H
#ضمن <malloc.h>
#د_که_پای

#ضمن "ext2_fs.h"
#تعریف_کړه INCLUDE_INLINE_FUNCS
#ضمن "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(بینښه اوږد اندازه,
			      بینښه اوږد align, فراغ *ptr)
{
	errcode_t retval = 0;
	فراغ **p = ptr;

	إذا (align < 8)
		align = 8;
#که_تعریف_شوی HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, اندازه);
	إذا (retval == ENOMEM)
		أرجع EXT2_ET_NO_حافظه;
#وإلا  /* !HAVE_POSIX_MEMALIGN */
#که_تعریف_شوی HAVE_MEMALIGN
	*p = memalign(align, اندازه);
	إذا (*p == عدم) {
		إذا (errno)
			أرجع errno;
		وإلا
			أرجع EXT2_ET_NO_حافظه;
	}
#وإلا  /* !HAVE_MEMALIGN */
#که_تعریف_شوی HAVE_VALLOC
	إذا (align > حجم(اوږد اوږد))
		*p = valloc(اندازه);
	وإلا
#د_که_پای
		*p = حافظه_ورکړه(اندازه);
	إذا ((uintptr_t) *p & (align - 1)) {
		حرر(*p);
		*p = 0;
	}
	إذا (*p == 0)
		أرجع EXT2_ET_NO_حافظه;
#د_که_پای	/* HAVE_MEMALIGN */
#د_که_پای	/* HAVE_POSIX_MEMALIGN */
	أرجع retval;
}

#که_تعریف_شوی DEBUG
ساكن صحيح isaligned(فراغ *ptr, بینښه اوږد align)
{
	أرجع (((بینښه اوږد) ptr & (align - 1)) == 0);
}

ساكن errcode_t test_memalign(بینښه اوږد align)
{
	فراغ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	إذا (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	حرر(ptr);
	چاپ_کړه("tst_memalign(%lu) is %s\n", align,
	       retval ? error_پیغام(retval) : "OK");
	أرجع retval;
}

صحيح اصلي(صحيح argc, حرف **argv)
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
#د_که_پای
