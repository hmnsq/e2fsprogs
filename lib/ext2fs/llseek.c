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
#ប្រសិនបើ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ប្រសិនបើ HAVE_ERRNO_H
#include <errno.h>
#endif
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (ចំនួនគត់ fd, ext2_loff_t offset, ចំនួនគត់ origin)
{
#ប្រសិនបើ SIZEOF_OFF_T >= 8
	ត្រឡប់ lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	ត្រឡប់ lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	ត្រឡប់ llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	ចំនួនគត់ retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	ត្រឡប់ (retval == -1 ? retval : result);
#បើមិនដូច្នេះ
	errno = ENOSYS;
	ត្រឡប់ -1;
#endif
}

ext2_loff_t ext2fs_llseek (ចំនួនគត់ fd, ext2_loff_t offset, ចំនួនគត់ origin)
{
	ext2_loff_t result;
	static ចំនួនគត់ do_compat = 0;

	ប្រសិនបើ (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	ប្រសិនបើ (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ប្រសិនបើ (offset < ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))
			ត្រឡប់ lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		ត្រឡប់ -1;
	}
	ត្រឡប់ result;
}

#បើមិនដូច្នេះ /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (ចំនួនគត់ fd, ext2_loff_t offset, ចំនួនគត់ origin)
{
#ប្រសិនបើ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ត្រឡប់ lseek64 (fd, offset, origin);
#បើមិនដូច្នេះ
	ប្រសិនបើ ((parimaa_na(off_t) < parimaa_na(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))) {
		errno = EINVAL;
		ត្រឡប់ -1;
	}
	ត្រឡប់ lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


