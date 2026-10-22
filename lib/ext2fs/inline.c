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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_MALLOC_H
#അന്തര്ഭാവ <malloc.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#അന്തര്ഭാവ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, ശൂന്യം *ptr)
{
	errcode_t retval = 0;
	ശൂന്യം **p = ptr;

	എങ്കിൽ (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	എങ്കിൽ (retval == ENOMEM)
		തിരികെ EXT2_ET_NO_MEMORY;
#അല്ലെങ്കിൽ  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	എങ്കിൽ (*p == ശൂന്യ) {
		എങ്കിൽ (errno)
			തിരികെ errno;
		അല്ലെങ്കിൽ
			തിരികെ EXT2_ET_NO_MEMORY;
	}
#അല്ലെങ്കിൽ  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	എങ്കിൽ (align > വലുപ്പം(long long))
		*p = valloc(size);
	അല്ലെങ്കിൽ
#endif
		*p = malloc(size);
	എങ്കിൽ ((uintptr_t) *p & (align - 1)) {
		സ്വതന്ത്രം(*p);
		*p = 0;
	}
	എങ്കിൽ (*p == 0)
		തിരികെ EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	തിരികെ retval;
}

#ifdef DEBUG
സ്ഥിതം പൂർണ്ണസംഖ്യ isaligned(ശൂന്യം *ptr, unsigned long align)
{
	തിരികെ (((unsigned long) ptr & (align - 1)) == 0);
}

സ്ഥിതം errcode_t test_memalign(unsigned long align)
{
	ശൂന്യം *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	എങ്കിൽ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	സ്വതന്ത്രം(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ err = 0;

	എങ്കിൽ (test_memalign(4))
		err++;
	എങ്കിൽ (test_memalign(32))
		err++;
	എങ്കിൽ (test_memalign(1024))
		err++;
	എങ്കിൽ (test_memalign(4096))
		err++;
	തിരികെ err;
}
#endif
