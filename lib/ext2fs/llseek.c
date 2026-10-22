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

#ضمن "config.h"
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ifdef __MSDOS__
#ضمن <io.h>
#endif
#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2_io.h"

#ifdef __linux__

#ضمن <sys/syscall.h>

ساكن ext2_loff_t my_llseek (صحيح fd, ext2_loff_t offset, صحيح origin)
{
#ܐܢ SIZEOF_OFF_T >= 8
	ܦܢܐ lseek(fd, offset, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	ܦܢܐ lseek64(fd, offset, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	ܦܢܐ llseek(fd, offset, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t result;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	ܦܢܐ (retval == -1 ? retval : result);
#ܐܚܪܢܐ
	errno = ENOSYS;
	ܦܢܐ -1;
#endif
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t offset, صحيح origin)
{
	ext2_loff_t result;
	ساكن صحيح do_compat = 0;

	ܐܢ (do_compat)
		اذهب fallback;

	result = my_llseek (fd, offset, origin);
	ܐܢ (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ܐܢ (offset < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			ܦܢܐ lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		ܦܢܐ -1;
	}
	ܦܢܐ result;
}

#ܐܚܪܢܐ /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t offset, صحيح origin)
{
#ܐܢ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ܦܢܐ lseek64 (fd, offset, origin);
#ܐܚܪܢܐ
	ܐܢ ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		ܦܢܐ -1;
	}
	ܦܢܐ lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


