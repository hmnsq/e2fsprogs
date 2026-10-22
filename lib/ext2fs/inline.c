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

#اگر_غیر_تعریف_شدہ XOPEN_ماخذ
#تعریف_کرو XOPEN_ماخذ 600	/* for posix_memalign() */
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_MALLOC_H
#شامل <malloc.h>
#اگر_ختم

#شامل "ext2_fs.h"
#تعریف_کرو INCLUDE_INLINE_FUNCS
#شامل "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(غیرعلامتی طویل سائز,
			      غیرعلامتی طویل align, خالی *ptr)
{
	errcode_t retval = 0;
	خالی **p = ptr;

	اگر (align < 8)
		align = 8;
#اگر_تعریف_شدہ HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, سائز);
	اگر (retval == ENOMEM)
		واپس EXT2_ET_NO_حافظہ;
#ورنہ  /* !HAVE_POSIX_MEMALIGN */
#اگر_تعریف_شدہ HAVE_MEMALIGN
	*p = memalign(align, سائز);
	اگر (*p == تہی) {
		اگر (errno)
			واپس errno;
		ورنہ
			واپس EXT2_ET_NO_حافظہ;
	}
#ورنہ  /* !HAVE_MEMALIGN */
#اگر_تعریف_شدہ HAVE_VALLOC
	اگر (align > حجم(طویل طویل))
		*p = valloc(سائز);
	ورنہ
#اگر_ختم
		*p = حافظہ_دو(سائز);
	اگر ((uintptr_t) *p & (align - 1)) {
		آزاد(*p);
		*p = 0;
	}
	اگر (*p == 0)
		واپس EXT2_ET_NO_حافظہ;
#اگر_ختم	/* HAVE_MEMALIGN */
#اگر_ختم	/* HAVE_POSIX_MEMALIGN */
	واپس retval;
}

#اگر_تعریف_شدہ DEBUG
ساکن صحیح isaligned(خالی *ptr, غیرعلامتی طویل align)
{
	واپس (((غیرعلامتی طویل) ptr & (align - 1)) == 0);
}

ساکن errcode_t test_memalign(غیرعلامتی طویل align)
{
	خالی *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	اگر (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	آزاد(ptr);
	لکھو("tst_memalign(%lu) is %s\n", align,
	       retval ? error_پیغام(retval) : "OK");
	واپس retval;
}

صحیح مین(صحیح argc, حرف **argv)
{
	صحیح err = 0;

	اگر (test_memalign(4))
		err++;
	اگر (test_memalign(32))
		err++;
	اگر (test_memalign(1024))
		err++;
	اگر (test_memalign(4096))
		err++;
	واپس err;
}
#اگر_ختم
