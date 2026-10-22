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

#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः

#अन्तर्भाव "config.h"
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि_परिभाषितम् __MSDOS__
#अन्तर्भाव <io.h>
#यदि_अन्तः
#अन्तर्भाव "et/com_err.h"
#अन्तर्भाव "ext2fs/ext2_io.h"

#यदि_परिभाषितम् __linux__

#अन्तर्भाव <sys/syscall.h>

स्थिर ext2_loff_t my_llseek (पूर्णाङ्क fd, ext2_loff_t विस्थापनम्, पूर्णाङ्क origin)
{
#यदि SIZEOF_OFF_T >= 8
	प्रत्यागम lseek(fd, विस्थापनम्, origin);
#नोचेद्यदि HAVE_LSEEK64_PROTOTYPE
	प्रत्यागम lseek64(fd, विस्थापनम्, origin);
#नोचेद्यदि HAVE_LLSEEK_PROTOTYPE
	प्रत्यागम llseek(fd, विस्थापनम्, origin);
#नोचेद्यदि defined(__NR__llseek)
	loff_t परिणाम;
	पूर्णाङ्क retval;
	retval = syscall(__NR__llseek, fd,
		(अचिह्नित दीर्घ)(विस्थापनम् >> 32),
		(अचिह्नित दीर्घ)(विस्थापनम् & 0xffffffff),
		&परिणाम, origin);
	प्रत्यागम (retval == -1 ? retval : परिणाम);
#अन्यथा
	errno = ENOSYS;
	प्रत्यागम -1;
#यदि_अन्तः
}

ext2_loff_t ext2fs_llseek (पूर्णाङ्क fd, ext2_loff_t विस्थापनम्, पूर्णाङ्क origin)
{
	ext2_loff_t परिणाम;
	स्थिर पूर्णाङ्क do_compat = 0;

	यदि (do_compat)
		गच्छ fallback;

	परिणाम = my_llseek (fd, विस्थापनम्, origin);
	यदि (परिणाम == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		यदि (विस्थापनम् < ((ext2_loff_t) 1 << ((परिमाण(off_t)*8) -1)))
			प्रत्यागम lseek(fd, (off_t) विस्थापनम्, origin);
		errno = EINVAL;
		प्रत्यागम -1;
	}
	प्रत्यागम परिणाम;
}

#अन्यथा /* !linux */

#यदि_अपरिभाषितम् EINVAL
#परिभाषय EINVAL EXT2_ET_INVALID_तर्कः
#यदि_अन्तः

ext2_loff_t ext2fs_llseek (पूर्णाङ्क fd, ext2_loff_t विस्थापनम्, पूर्णाङ्क origin)
{
#यदि defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	प्रत्यागम lseek64 (fd, विस्थापनम्, origin);
#अन्यथा
	यदि ((परिमाण(off_t) < परिमाण(ext2_loff_t)) &&
	    (विस्थापनम् >= ((ext2_loff_t) 1 << ((परिमाण(off_t)*8) -1)))) {
		errno = EINVAL;
		प्रत्यागम -1;
	}
	प्रत्यागम lseek (fd, (off_t) विस्थापनम्, origin);
#यदि_अन्तः
}

#यदि_अन्तः 	/* linux */


