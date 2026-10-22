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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ifdef __MSDOS__
#ಅನ್ತರ್ಭಾವ <io.h>
#endif
#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_io.h"

#ifdef __linux__

#ಅನ್ತರ್ಭಾವ <sys/syscall.h>

ಸ್ಥಿರ ext2_loff_t my_llseek (ಪೂರ್ಣಾಙ್ಕ fd, ext2_loff_t offset, ಪೂರ್ಣಾಙ್ಕ origin)
{
#ಯದಿ SIZEOF_OFF_T >= 8
	ಪ್ರತ್ಯಾಗಮ lseek(fd, offset, origin);
#ನೋಚೇದ್ಯದಿ HAVE_LSEEK64_PROTOTYPE
	ಪ್ರತ್ಯಾಗಮ lseek64(fd, offset, origin);
#ನೋಚೇದ್ಯದಿ HAVE_LLSEEK_PROTOTYPE
	ಪ್ರತ್ಯಾಗಮ llseek(fd, offset, origin);
#ನೋಚೇದ್ಯದಿ defined(__NR__llseek)
	loff_t result;
	ಪೂರ್ಣಾಙ್ಕ retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	ಪ್ರತ್ಯಾಗಮ (retval == -1 ? retval : result);
#ಅನ್ಯಥಾ
	errno = ENOSYS;
	ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ext2_loff_t ext2fs_llseek (ಪೂರ್ಣಾಙ್ಕ fd, ext2_loff_t offset, ಪೂರ್ಣಾಙ್ಕ origin)
{
	ext2_loff_t result;
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_compat = 0;

	ಯದಿ (do_compat)
		ಗಚ್ಛ fallback;

	result = my_llseek (fd, offset, origin);
	ಯದಿ (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ಯದಿ (offset < ((ext2_loff_t) 1 << ((ಪರಿಮಾಣ(off_t)*8) -1)))
			ಪ್ರತ್ಯಾಗಮ lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಪ್ರತ್ಯಾಗಮ result;
}

#ಅನ್ಯಥಾ /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (ಪೂರ್ಣಾಙ್ಕ fd, ext2_loff_t offset, ಪೂರ್ಣಾಙ್ಕ origin)
{
#ಯದಿ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ಪ್ರತ್ಯಾಗಮ lseek64 (fd, offset, origin);
#ಅನ್ಯಥಾ
	ಯದಿ ((ಪರಿಮಾಣ(off_t) < ಪರಿಮಾಣ(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((ಪರಿಮಾಣ(off_t)*8) -1)))) {
		errno = EINVAL;
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಪ್ರತ್ಯಾಗಮ lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


