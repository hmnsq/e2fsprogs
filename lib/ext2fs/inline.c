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

#यदि_अपरिभाषितम् _XOPEN_SOURCE
#परिभाषय _XOPEN_SOURCE 600	/* for posix_memalign() */
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_MALLOC_H
#अन्तर्भाव <malloc.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#परिभाषय INCLUDE_INLINE_FUNCS
#अन्तर्भाव "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(अचिह्नित दीर्घ आकार,
			      अचिह्नित दीर्घ align, शून्य *ptr)
{
	errcode_t retval = 0;
	शून्य **p = ptr;

	यदि (align < 8)
		align = 8;
#यदि_परिभाषितम् HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, आकार);
	यदि (retval == ENOMEM)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
#अन्यथा  /* !HAVE_POSIX_MEMALIGN */
#यदि_परिभाषितम् HAVE_MEMALIGN
	*p = memalign(align, आकार);
	यदि (*p == रिक्त) {
		यदि (errno)
			प्रत्यागम errno;
		अन्यथा
			प्रत्यागम EXT2_ET_NO_स्मृतिः;
	}
#अन्यथा  /* !HAVE_MEMALIGN */
#यदि_परिभाषितम् HAVE_VALLOC
	यदि (align > परिमाण(दीर्घ दीर्घ))
		*p = valloc(आकार);
	अन्यथा
#यदि_अन्तः
		*p = स्मृतिं_देहि(आकार);
	यदि ((uintptr_t) *p & (align - 1)) {
		मुक्त(*p);
		*p = 0;
	}
	यदि (*p == 0)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
#यदि_अन्तः	/* HAVE_MEMALIGN */
#यदि_अन्तः	/* HAVE_POSIX_MEMALIGN */
	प्रत्यागम retval;
}

#यदि_परिभाषितम् DEBUG
स्थिर पूर्णाङ्क isaligned(शून्य *ptr, अचिह्नित दीर्घ align)
{
	प्रत्यागम (((अचिह्नित दीर्घ) ptr & (align - 1)) == 0);
}

स्थिर errcode_t test_memalign(अचिह्नित दीर्घ align)
{
	शून्य *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	यदि (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	मुक्त(ptr);
	प्रारूप_लिख("tst_memalign(%lu) is %s\n", align,
	       retval ? error_सन्देशः(retval) : "OK");
	प्रत्यागम retval;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क err = 0;

	यदि (test_memalign(4))
		err++;
	यदि (test_memalign(32))
		err++;
	यदि (test_memalign(1024))
		err++;
	यदि (test_memalign(4096))
		err++;
	प्रत्यागम err;
}
#यदि_अन्तः
