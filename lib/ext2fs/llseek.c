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
#만약 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#만약 HAVE_ERRNO_H
#include <errno.h>
#endif
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (정수 fd, ext2_loff_t offset, 정수 origin)
{
#만약 SIZEOF_OFF_T >= 8
	반환 lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	반환 lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	반환 llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	정수 retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	반환 (retval == -1 ? retval : result);
#아니면
	errno = ENOSYS;
	반환 -1;
#endif
}

ext2_loff_t ext2fs_llseek (정수 fd, ext2_loff_t offset, 정수 origin)
{
	ext2_loff_t result;
	static 정수 do_compat = 0;

	만약 (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	만약 (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		만약 (offset < ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))
			반환 lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		반환 -1;
	}
	반환 result;
}

#아니면 /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (정수 fd, ext2_loff_t offset, 정수 origin)
{
#만약 defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	반환 lseek64 (fd, offset, origin);
#아니면
	만약 ((sizeof(off_t) < sizeof(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((sizeof(off_t)*8) -1)))) {
		errno = EINVAL;
		반환 -1;
	}
	반환 lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


