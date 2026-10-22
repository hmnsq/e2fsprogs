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

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#अन्तर्भाव "config.h"
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#ifdef __MSDOS__
#अन्तर्भाव <io.h>
#endif
#अन्तर्भाव "et/com_err.h"
#अन्तर्भाव "ext2fs/ext2_io.h"

#ifdef __linux__

#अन्तर्भाव <sys/syscall.h>

स्थिर ext2_loff_t my_llseek (पूर्णाङ्क fd, ext2_loff_t offset, पूर्णाङ्क origin)
{
#यदि SIZEOF_OFF_T >= 8
	प्रत्यागम lseek(fd, offset, origin);
#नोचेद्यदि HAVE_LSEEK64_PROTOTYPE
	प्रत्यागम lseek64(fd, offset, origin);
#नोचेद्यदि HAVE_LLSEEK_PROTOTYPE
	प्रत्यागम llseek(fd, offset, origin);
#नोचेद्यदि defined(__NR__llseek)
	loff_t result;
	पूर्णाङ्क retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	प्रत्यागम (retval == -1 ? retval : result);
#अन्यथा
	errno = ENOSYS;
	प्रत्यागम -1;
#endif
}

ext2_loff_t ext2fs_llseek (पूर्णाङ्क fd, ext2_loff_t offset, पूर्णाङ्क origin)
{
	ext2_loff_t result;
	स्थिर पूर्णाङ्क do_compat = 0;

	यदि (do_compat)
		गच्छ fallback;

	result = my_llseek (fd, offset, origin);
	यदि (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		यदि (offset < ((ext2_loff_t) 1 << ((परिमाण(off_t)*8) -1)))
			प्रत्यागम lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		प्रत्यागम -1;
	}
	प्रत्यागम result;
}

#अन्यथा /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (पूर्णाङ्क fd, ext2_loff_t offset, पूर्णाङ्क origin)
{
#यदि defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	प्रत्यागम lseek64 (fd, offset, origin);
#अन्यथा
	यदि ((परिमाण(off_t) < परिमाण(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((परिमाण(off_t)*8) -1)))) {
		errno = EINVAL;
		प्रत्यागम -1;
	}
	प्रत्यागम lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


