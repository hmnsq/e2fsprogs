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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_MALLOC_H
#அந்தர்பாவ <malloc.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#அந்தர்பாவ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, வெறுமை *ptr)
{
	errcode_t retval = 0;
	வெறுமை **p = ptr;

	எனில் (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	எனில் (retval == ENOMEM)
		திரும்பு EXT2_ET_NO_MEMORY;
#இல்லையெனில்  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	எனில் (*p == வெற்று) {
		எனில் (errno)
			திரும்பு errno;
		இல்லையெனில்
			திரும்பு EXT2_ET_NO_MEMORY;
	}
#இல்லையெனில்  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	எனில் (align > அளவு(long long))
		*p = valloc(size);
	இல்லையெனில்
#endif
		*p = malloc(size);
	எனில் ((uintptr_t) *p & (align - 1)) {
		விடுவி(*p);
		*p = 0;
	}
	எனில் (*p == 0)
		திரும்பு EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	திரும்பு retval;
}

#ifdef DEBUG
நிலையான முழு isaligned(வெறுமை *ptr, unsigned long align)
{
	திரும்பு (((unsigned long) ptr & (align - 1)) == 0);
}

நிலையான errcode_t test_memalign(unsigned long align)
{
	வெறுமை *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	எனில் (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	விடுவி(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	திரும்பு retval;
}

முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	முழு err = 0;

	எனில் (test_memalign(4))
		err++;
	எனில் (test_memalign(32))
		err++;
	எனில் (test_memalign(1024))
		err++;
	எனில் (test_memalign(4096))
		err++;
	திரும்பு err;
}
#endif
