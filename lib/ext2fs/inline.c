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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#सचे HAVE_MALLOC_H
#अन्तर्भाव <malloc.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#define INCLUDE_INLINE_FUNCS
#अन्तर्भाव "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(unsigned long size,
			      unsigned long align, शून्य *ptr)
{
	errcode_t retval = 0;
	शून्य **p = ptr;

	सचे (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	सचे (retval == ENOMEM)
		पटिनिवत्त EXT2_ET_NO_MEMORY;
#अञ्ञथा  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	सचे (*p == रिक्त) {
		सचे (errno)
			पटिनिवत्त errno;
		अञ्ञथा
			पटिनिवत्त EXT2_ET_NO_MEMORY;
	}
#अञ्ञथा  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	सचे (align > परिमाण(long long))
		*p = valloc(size);
	अञ्ञथा
#endif
		*p = malloc(size);
	सचे ((uintptr_t) *p & (align - 1)) {
		मुक्त(*p);
		*p = 0;
	}
	सचे (*p == 0)
		पटिनिवत्त EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	पटिनिवत्त retval;
}

#ifdef DEBUG
स्थिर पूर्णाङ्क isaligned(शून्य *ptr, unsigned long align)
{
	पटिनिवत्त (((unsigned long) ptr & (align - 1)) == 0);
}

स्थिर errcode_t test_memalign(unsigned long align)
{
	शून्य *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	सचे (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	मुक्त(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	पटिनिवत्त retval;
}

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क err = 0;

	सचे (test_memalign(4))
		err++;
	सचे (test_memalign(32))
		err++;
	सचे (test_memalign(1024))
		err++;
	सचे (test_memalign(4096))
		err++;
	पटिनिवत्त err;
}
#endif
