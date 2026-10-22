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

#جيڪڏهن_وصف_نه_ٿيل XOPEN_ذريعو
#وصف_ڪريو XOPEN_ذريعو 600	/* for posix_memalign() */
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_MALLOC_H
#ضمن <malloc.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#وصف_ڪريو INCLUDE_INLINE_FUNCS
#ضمن "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(بنانشان ڊگهو سائيز,
			      بنانشان ڊگهو align, فراغ *ptr)
{
	errcode_t retval = 0;
	فراغ **p = ptr;

	إذا (align < 8)
		align = 8;
#جيڪڏهن_وصف_ٿيل HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, سائيز);
	إذا (retval == ENOMEM)
		أرجع EXT2_ET_NO_يادگيري;
#وإلا  /* !HAVE_POSIX_MEMALIGN */
#جيڪڏهن_وصف_ٿيل HAVE_MEMALIGN
	*p = memalign(align, سائيز);
	إذا (*p == عدم) {
		إذا (errno)
			أرجع errno;
		وإلا
			أرجع EXT2_ET_NO_يادگيري;
	}
#وإلا  /* !HAVE_MEMALIGN */
#جيڪڏهن_وصف_ٿيل HAVE_VALLOC
	إذا (align > حجم(ڊگهو ڊگهو))
		*p = valloc(سائيز);
	وإلا
#جيڪڏهن_پورو
		*p = يادگيري_ڏيو(سائيز);
	إذا ((uintptr_t) *p & (align - 1)) {
		حرر(*p);
		*p = 0;
	}
	إذا (*p == 0)
		أرجع EXT2_ET_NO_يادگيري;
#جيڪڏهن_پورو	/* HAVE_MEMALIGN */
#جيڪڏهن_پورو	/* HAVE_POSIX_MEMALIGN */
	أرجع retval;
}

#جيڪڏهن_وصف_ٿيل DEBUG
ساكن صحيح isaligned(فراغ *ptr, بنانشان ڊگهو align)
{
	أرجع (((بنانشان ڊگهو) ptr & (align - 1)) == 0);
}

ساكن errcode_t test_memalign(بنانشان ڊگهو align)
{
	فراغ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	إذا (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	حرر(ptr);
	لکو("tst_memalign(%lu) is %s\n", align,
	       retval ? error_پيغام(retval) : "OK");
	أرجع retval;
}

صحيح مکيه(صحيح argc, حرف **argv)
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
#جيڪڏهن_پورو
