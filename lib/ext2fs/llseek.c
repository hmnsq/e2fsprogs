/*
 * llseek.c -- stub calling the llseek system call
 *
 * Copyright (C) 1994, 1995, 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर_घोषित __MSDOS__
#समावेश <io.h>
#पूर्ण_अगर
#समावेश "et/com_err.h"
#समावेश "ext2fs/ext2_io.h"

#अगर_घोषित __linux__

#समावेश <sys/syscall.h>

अटल ext2_loff_t my_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
#अगर SIZEOF_OFF_T >= 8
	वापस lseek(fd, offset, origin);
#या_अगर HAVE_LSEEK64_PROTOTYPE
	वापस lseek64(fd, offset, origin);
#या_अगर HAVE_LLSEEK_PROTOTYPE
	वापस llseek(fd, offset, origin);
#या_अगर defined(__NR__llseek)
	loff_t परिणाम;
	पूर्णांक retval;
	retval = syscall(__NR__llseek, fd,
		(अचिन्हित दीर्घ)(offset >> 32),
		(अचिन्हित दीर्घ)(offset & 0xffffffff),
		&परिणाम, origin);
	वापस (retval == -1 ? retval : परिणाम);
#अन्यथा
	त्रुटि_सं = ENOSYS;
	वापस -1;
#पूर्ण_अगर
}

ext2_loff_t ext2fs_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
	ext2_loff_t परिणाम;
	अटल पूर्णांक do_compat = 0;

	अगर (do_compat)
		जाओ fallback;

	परिणाम = my_llseek (fd, offset, origin);
	अगर (परिणाम == -1 && त्रुटि_सं == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		अगर (offset < ((ext2_loff_t) 1 << ((माप(off_t)*8) -1)))
			वापस lseek(fd, (off_t) offset, origin);
		त्रुटि_सं = EINVAL;
		वापस -1;
	}
	वापस परिणाम;
}

#अन्यथा /* !linux */

#अगर_अघोषित EINVAL
#घोषणा EINVAL EXT2_ET_INVALID_ARGUMENT
#पूर्ण_अगर

ext2_loff_t ext2fs_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
#अगर defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	वापस lseek64 (fd, offset, origin);
#अन्यथा
	अगर ((माप(off_t) < माप(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((माप(off_t)*8) -1)))) {
		त्रुटि_सं = EINVAL;
		वापस -1;
	}
	वापस lseek (fd, (off_t) offset, origin);
#पूर्ण_अगर
}

#पूर्ण_अगर 	/* linux */


