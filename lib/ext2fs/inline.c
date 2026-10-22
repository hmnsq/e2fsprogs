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

#جے_بنا_تعریف XOPEN_سروت
#تعریف_کرو XOPEN_سروت 600	/* for posix_memalign() */
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_MALLOC_H
#ضمن <malloc.h>
#جے_انت

#ضمن "ext2_fs.h"
#تعریف_کرو INCLUDE_INLINE_FUNCS
#ضمن "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(بنانشانی لما ناپ,
			      بنانشانی لما align, فراغ *ptr)
{
	errcode_t retval = 0;
	فراغ **p = ptr;

	إذا (align < 8)
		align = 8;
#جے_تعریف_شدہ HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, ناپ);
	إذا (retval == ENOMEM)
		أرجع EXT2_ET_NO_حافظہ;
#وإلا  /* !HAVE_POSIX_MEMALIGN */
#جے_تعریف_شدہ HAVE_MEMALIGN
	*p = memalign(align, ناپ);
	إذا (*p == عدم) {
		إذا (errno)
			أرجع errno;
		وإلا
			أرجع EXT2_ET_NO_حافظہ;
	}
#وإلا  /* !HAVE_MEMALIGN */
#جے_تعریف_شدہ HAVE_VALLOC
	إذا (align > حجم(لما لما))
		*p = valloc(ناپ);
	وإلا
#جے_انت
		*p = حافظہ_دیو(ناپ);
	إذا ((uintptr_t) *p & (align - 1)) {
		حرر(*p);
		*p = 0;
	}
	إذا (*p == 0)
		أرجع EXT2_ET_NO_حافظہ;
#جے_انت	/* HAVE_MEMALIGN */
#جے_انت	/* HAVE_POSIX_MEMALIGN */
	أرجع retval;
}

#جے_تعریف_شدہ DEBUG
ساكن صحيح isaligned(فراغ *ptr, بنانشانی لما align)
{
	أرجع (((بنانشانی لما) ptr & (align - 1)) == 0);
}

ساكن errcode_t test_memalign(بنانشانی لما align)
{
	فراغ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	إذا (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	حرر(ptr);
	لکھو("tst_memalign(%lu) is %s\n", align,
	       retval ? error_سنیہا(retval) : "OK");
	أرجع retval;
}

صحيح مکھ(صحيح argc, حرف **argv)
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
#جے_انت
