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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ XOPEN_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ 600	/* for posix_memalign() */
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_MALLOC_H
#ਅਨ੍ਤਰ੍ਭਾਵ <malloc.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_INLINE_FUNCS
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ align, ਖਾਲੀ *ptr)
{
	errcode_t retval = 0;
	ਖਾਲੀ **p = ptr;

	ਜੇ (align < 8)
		align = 8;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, ਮਾਪ);
	ਜੇ (retval == ENOMEM)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
#ਨਹੀਂਤਾਂ  /* !HAVE_POSIX_MEMALIGN */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MEMALIGN
	*p = memalign(align, ਮਾਪ);
	ਜੇ (*p == ਨਿਰਾ) {
		ਜੇ (errno)
			ਵਾਪਸ errno;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	}
#ਨਹੀਂਤਾਂ  /* !HAVE_MEMALIGN */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VALLOC
	ਜੇ (align > ਆਕਾਰ(ਲੰਮਾ ਲੰਮਾ))
		*p = valloc(ਮਾਪ);
	ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
		*p = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
	ਜੇ ((uintptr_t) *p & (align - 1)) {
		ਮੁਕਤ(*p);
		*p = 0;
	}
	ਜੇ (*p == 0)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
#ਜੇ_ਅੰਤ	/* HAVE_MEMALIGN */
#ਜੇ_ਅੰਤ	/* HAVE_POSIX_MEMALIGN */
	ਵਾਪਸ retval;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਸਥਿਰ ਪੂਰਨ isaligned(ਖਾਲੀ *ptr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ align)
{
	ਵਾਪਸ (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ptr & (align - 1)) == 0);
}

ਸਥਿਰ errcode_t test_memalign(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ align)
{
	ਖਾਲੀ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ਜੇ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	ਮੁਕਤ(ptr);
	ਲਿਖੋ("tst_memalign(%lu) is %s\n", align,
	       retval ? error_ਸੁਨੇਹਾ(retval) : "OK");
	ਵਾਪਸ retval;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ err = 0;

	ਜੇ (test_memalign(4))
		err++;
	ਜੇ (test_memalign(32))
		err++;
	ਜੇ (test_memalign(1024))
		err++;
	ਜੇ (test_memalign(4096))
		err++;
	ਵਾਪਸ err;
}
#ਜੇ_ਅੰਤ
