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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जइ HAVE_MALLOC_H
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

	जइ (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	जइ (retval == ENOMEM)
		प्रत्यागम EXT2_ET_NO_MEMORY;
#अण्णहा  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	जइ (*p == रिक्त) {
		जइ (errno)
			प्रत्यागम errno;
		अण्णहा
			प्रत्यागम EXT2_ET_NO_MEMORY;
	}
#अण्णहा  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	जइ (align > परिमाण(long long))
		*p = valloc(size);
	अण्णहा
#endif
		*p = malloc(size);
	जइ ((uintptr_t) *p & (align - 1)) {
		मुक्त(*p);
		*p = 0;
	}
	जइ (*p == 0)
		प्रत्यागम EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	प्रत्यागम retval;
}

#ifdef DEBUG
स्थिर पूर्णाङ्क isaligned(शून्य *ptr, unsigned long align)
{
	प्रत्यागम (((unsigned long) ptr & (align - 1)) == 0);
}

स्थिर errcode_t test_memalign(unsigned long align)
{
	शून्य *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	जइ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	मुक्त(ptr);
	printf("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	प्रत्यागम retval;
}

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क err = 0;

	जइ (test_memalign(4))
		err++;
	जइ (test_memalign(32))
		err++;
	जइ (test_memalign(1024))
		err++;
	जइ (test_memalign(4096))
		err++;
	प्रत्यागम err;
}
#endif
