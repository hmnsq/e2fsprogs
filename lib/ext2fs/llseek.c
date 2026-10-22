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
#もし HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#もし HAVE_ERRNO_H
#include <errno.h>
#endif
#もし HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (整数 fd, ext2_loff_t offset, 整数 origin)
{
#もし SIZEOF_OFF_T >= 8
	戻る lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	戻る lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	戻る llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	整数 retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	戻る (retval == -1 ? retval : result);
#否则
	errno = ENOSYS;
	戻る -1;
#endif
}

ext2_loff_t ext2fs_llseek (整数 fd, ext2_loff_t offset, 整数 origin)
{
	ext2_loff_t result;
	static 整数 do_compat = 0;

	もし (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	もし (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		もし (offset < ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))
			戻る lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		戻る -1;
	}
	戻る result;
}

#否则 /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (整数 fd, ext2_loff_t offset, 整数 origin)
{
#もし defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	戻る lseek64 (fd, offset, origin);
#否则
	もし ((sizeof(off_t) < sizeof(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))) {
		errno = EINVAL;
		戻る -1;
	}
	戻る lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


