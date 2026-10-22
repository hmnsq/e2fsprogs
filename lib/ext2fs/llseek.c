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

#অন্তর্ভাব "config.h"
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#ifdef __MSDOS__
#অন্তর্ভাব <io.h>
#endif
#অন্তর্ভাব "et/com_err.h"
#অন্তর্ভাব "ext2fs/ext2_io.h"

#ifdef __linux__

#অন্তর্ভাব <sys/syscall.h>

স্থির ext2_loff_t my_llseek (পূর্ণাঙ্ক fd, ext2_loff_t offset, পূর্ণাঙ্ক origin)
{
#যদি SIZEOF_OFF_T >= 8
	প্রত্যাগম lseek(fd, offset, origin);
#নোচেদ্যদি HAVE_LSEEK64_PROTOTYPE
	প্রত্যাগম lseek64(fd, offset, origin);
#নোচেদ্যদি HAVE_LLSEEK_PROTOTYPE
	প্রত্যাগম llseek(fd, offset, origin);
#নোচেদ্যদি defined(__NR__llseek)
	loff_t result;
	পূর্ণাঙ্ক retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	প্রত্যাগম (retval == -1 ? retval : result);
#অন্যথা
	errno = ENOSYS;
	প্রত্যাগম -1;
#endif
}

ext2_loff_t ext2fs_llseek (পূর্ণাঙ্ক fd, ext2_loff_t offset, পূর্ণাঙ্ক origin)
{
	ext2_loff_t result;
	স্থির পূর্ণাঙ্ক do_compat = 0;

	যদি (do_compat)
		গচ্ছ fallback;

	result = my_llseek (fd, offset, origin);
	যদি (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		যদি (offset < ((ext2_loff_t) 1 << ((পরিমাণ(off_t)*8) -1)))
			প্রত্যাগম lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		প্রত্যাগম -1;
	}
	প্রত্যাগম result;
}

#অন্যথা /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (পূর্ণাঙ্ক fd, ext2_loff_t offset, পূর্ণাঙ্ক origin)
{
#যদি defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	প্রত্যাগম lseek64 (fd, offset, origin);
#অন্যথা
	যদি ((পরিমাণ(off_t) < পরিমাণ(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((পরিমাণ(off_t)*8) -1)))) {
		errno = EINVAL;
		প্রত্যাগম -1;
	}
	প্রত্যাগম lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


