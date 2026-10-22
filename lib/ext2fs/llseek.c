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
#ถ้า HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ถ้า HAVE_ERRNO_H
#include <errno.h>
#endif
#ถ้า HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (จำนวนเต็ม fd, ext2_loff_t offset, จำนวนเต็ม origin)
{
#ถ้า SIZEOF_OFF_T >= 8
	คืน lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	คืน lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	คืน llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	จำนวนเต็ม retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	คืน (retval == -1 ? retval : result);
#มิฉะนั้น
	errno = ENOSYS;
	คืน -1;
#endif
}

ext2_loff_t ext2fs_llseek (จำนวนเต็ม fd, ext2_loff_t offset, จำนวนเต็ม origin)
{
	ext2_loff_t result;
	static จำนวนเต็ม do_compat = 0;

	ถ้า (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	ถ้า (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ถ้า (offset < ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))
			คืน lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		คืน -1;
	}
	คืน result;
}

#มิฉะนั้น /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (จำนวนเต็ม fd, ext2_loff_t offset, จำนวนเต็ม origin)
{
#ถ้า defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	คืน lseek64 (fd, offset, origin);
#มิฉะนั้น
	ถ้า ((parimaa_na(off_t) < parimaa_na(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))) {
		errno = EINVAL;
		คืน -1;
	}
	คืน lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


