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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जर HAVE_MALLOC_H
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
errcode_t ext2fs_get_memalign(अचिन्हित दीर्घ size,
			      अचिन्हित दीर्घ align, शून्य *ptr)
{
	errcode_t retval = 0;
	शून्य **p = ptr;

	जर (align < 8)
		align = 8;
#ifdef HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, size);
	जर (retval == ENOMEM)
		परत EXT2_ET_NO_MEMORY;
#अन्यथा  /* !HAVE_POSIX_MEMALIGN */
#ifdef HAVE_MEMALIGN
	*p = memalign(align, size);
	जर (*p == रिक्त) {
		जर (त्रुटि_सं)
			परत त्रुटि_सं;
		अन्यथा
			परत EXT2_ET_NO_MEMORY;
	}
#अन्यथा  /* !HAVE_MEMALIGN */
#ifdef HAVE_VALLOC
	जर (align > आकार(दीर्घ दीर्घ))
		*p = valloc(size);
	अन्यथा
#endif
		*p = दो_स्मृति(size);
	जर ((uintptr_t) *p & (align - 1)) {
		मुक्त(*p);
		*p = 0;
	}
	जर (*p == 0)
		परत EXT2_ET_NO_MEMORY;
#endif	/* HAVE_MEMALIGN */
#endif	/* HAVE_POSIX_MEMALIGN */
	परत retval;
}

#ifdef DEBUG
स्थिर पूर्णांक isaligned(शून्य *ptr, अचिन्हित दीर्घ align)
{
	परत (((अचिन्हित दीर्घ) ptr & (align - 1)) == 0);
}

स्थिर errcode_t test_memalign(अचिन्हित दीर्घ align)
{
	शून्य *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	जर (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	मुक्त(ptr);
	म_लिखो("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	परत retval;
}

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक err = 0;

	जर (test_memalign(4))
		err++;
	जर (test_memalign(32))
		err++;
	जर (test_memalign(1024))
		err++;
	जर (test_memalign(4096))
		err++;
	परत err;
}
#endif
