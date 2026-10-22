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

#अगर_अघोषित _XOPEN_SOURCE
#घोषणा _XOPEN_SOURCE 600	/* for posix_memalign() */
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर HAVE_MALLOC_H
#समावेश <malloc.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#घोषणा INCLUDE_INLINE_FUNCS
#समावेश "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(अचिन्हित दीर्घ आकार,
			      अचिन्हित दीर्घ align, व्योम *ptr)
{
	errcode_t retval = 0;
	व्योम **p = ptr;

	अगर (align < 8)
		align = 8;
#अगर_घोषित HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, आकार);
	अगर (retval == ENOMEM)
		वापस EXT2_ET_NO_MEMORY;
#अन्यथा  /* !HAVE_POSIX_MEMALIGN */
#अगर_घोषित HAVE_MEMALIGN
	*p = memalign(align, आकार);
	अगर (*p == शून्य) {
		अगर (त्रुटि_सं)
			वापस त्रुटि_सं;
		अन्यथा
			वापस EXT2_ET_NO_MEMORY;
	}
#अन्यथा  /* !HAVE_MEMALIGN */
#अगर_घोषित HAVE_VALLOC
	अगर (align > माप(दीर्घ दीर्घ))
		*p = valloc(आकार);
	अन्यथा
#पूर्ण_अगर
		*p = दो_स्मृति(आकार);
	अगर ((uintptr_t) *p & (align - 1)) {
		मुक्त(*p);
		*p = 0;
	}
	अगर (*p == 0)
		वापस EXT2_ET_NO_MEMORY;
#पूर्ण_अगर	/* HAVE_MEMALIGN */
#पूर्ण_अगर	/* HAVE_POSIX_MEMALIGN */
	वापस retval;
}

#अगर_घोषित DEBUG
अटल पूर्णांक isaligned(व्योम *ptr, अचिन्हित दीर्घ align)
{
	वापस (((अचिन्हित दीर्घ) ptr & (align - 1)) == 0);
}

अटल errcode_t test_memalign(अचिन्हित दीर्घ align)
{
	व्योम *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	अगर (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	मुक्त(ptr);
	म_लिखो("tst_memalign(%lu) is %s\n", align,
	       retval ? error_message(retval) : "OK");
	वापस retval;
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक err = 0;

	अगर (test_memalign(4))
		err++;
	अगर (test_memalign(32))
		err++;
	अगर (test_memalign(1024))
		err++;
	अगर (test_memalign(4096))
		err++;
	वापस err;
}
#पूर्ण_अगर
