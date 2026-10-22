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
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#အကယ်၍ HAVE_ERRNO_H
#include <errno.h>
#endif
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef __MSDOS__
#include <io.h>
#endif
#include "et/com_err.h"
#include "ext2fs/ext2_io.h"

#ifdef __linux__

#include <sys/syscall.h>

static ext2_loff_t my_llseek (ကိန်းပြည့် fd, ext2_loff_t offset, ကိန်းပြည့် origin)
{
#အကယ်၍ SIZEOF_OFF_T >= 8
	ပြန်ပေး lseek(fd, offset, origin);
#elif HAVE_LSEEK64_PROTOTYPE
	ပြန်ပေး lseek64(fd, offset, origin);
#elif HAVE_LLSEEK_PROTOTYPE
	ပြန်ပေး llseek(fd, offset, origin);
#elif defined(__NR__llseek)
	loff_t result;
	ကိန်းပြည့် retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	ပြန်ပေး (retval == -1 ? retval : result);
#မဟုတ်ပါက
	errno = ENOSYS;
	ပြန်ပေး -1;
#endif
}

ext2_loff_t ext2fs_llseek (ကိန်းပြည့် fd, ext2_loff_t offset, ကိန်းပြည့် origin)
{
	ext2_loff_t result;
	static ကိန်းပြည့် do_compat = 0;

	အကယ်၍ (do_compat)
		goto fallback;

	result = my_llseek (fd, offset, origin);
	အကယ်၍ (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		အကယ်၍ (offset < ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))
			ပြန်ပေး lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		ပြန်ပေး -1;
	}
	ပြန်ပေး result;
}

#မဟုတ်ပါက /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (ကိန်းပြည့် fd, ext2_loff_t offset, ကိန်းပြည့် origin)
{
#အကယ်၍ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ပြန်ပေး lseek64 (fd, offset, origin);
#မဟုတ်ပါက
	အကယ်၍ ((parimaa_na(off_t) < parimaa_na(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((parimaa_na(off_t)*8) -1)))) {
		errno = EINVAL;
		ပြန်ပေး -1;
	}
	ပြန်ပေး lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


