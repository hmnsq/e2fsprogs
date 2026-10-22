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

#شامل "config.h"
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#ifdef __MSDOS__
#شامل <io.h>
#endif
#شامل "et/com_err.h"
#شامل "ext2fs/ext2_io.h"

#ifdef __linux__

#شامل <sys/syscall.h>

ایستا ext2_loff_t my_llseek (صحیح fd, ext2_loff_t offset, صحیح origin)
{
#اگر SIZEOF_OFF_T >= 8
	بازگشت lseek(fd, offset, origin);
#وگرنهاگر HAVE_LSEEK64_PROTOTYPE
	بازگشت lseek64(fd, offset, origin);
#وگرنهاگر HAVE_LLSEEK_PROTOTYPE
	بازگشت llseek(fd, offset, origin);
#وگرنهاگر defined(__NR__llseek)
	loff_t result;
	صحیح retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	بازگشت (retval == -1 ? retval : result);
#وگرنه
	errno = ENOSYS;
	بازگشت -1;
#endif
}

ext2_loff_t ext2fs_llseek (صحیح fd, ext2_loff_t offset, صحیح origin)
{
	ext2_loff_t result;
	ایستا صحیح do_compat = 0;

	اگر (do_compat)
		برو fallback;

	result = my_llseek (fd, offset, origin);
	اگر (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		اگر (offset < ((ext2_loff_t) 1 << ((اندازه(off_t)*8) -1)))
			بازگشت lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		بازگشت -1;
	}
	بازگشت result;
}

#وگرنه /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (صحیح fd, ext2_loff_t offset, صحیح origin)
{
#اگر defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	بازگشت lseek64 (fd, offset, origin);
#وگرنه
	اگر ((اندازه(off_t) < اندازه(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((اندازه(off_t)*8) -1)))) {
		errno = EINVAL;
		بازگشت -1;
	}
	بازگشت lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


