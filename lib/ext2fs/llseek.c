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

#include "config.h"
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#nếu HAVE_ERRNO_H
#include <errno.h>
#endif
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (số_nguyên fd, ext2_loff_t offset, số_nguyên origin)
{
#nếu SIZEOF_OFF_T >= 8
	trả_về lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	trả_về lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	trả_về llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	số_nguyên retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	trả_về (retval == -1 ? retval : result);
#không_thì
	errno = ENOSYS;
	trả_về -1;
#endif
}

ext2_loff_t ext2fs_llseek (số_nguyên fd, ext2_loff_t offset, số_nguyên origin)
{
	ext2_loff_t result;
	static số_nguyên do_compat = 0;

	nếu (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	nếu (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		nếu (offset < ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))
			trả_về lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		trả_về -1;
	}
	trả_về result;
}

#không_thì /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (số_nguyên fd, ext2_loff_t offset, số_nguyên origin)
{
#nếu defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	trả_về lseek64 (fd, offset, origin);
#không_thì
	nếu ((sizeof(off_t) < sizeof(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))) {
		errno = EINVAL;
		trả_về -1;
	}
	trả_về lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


