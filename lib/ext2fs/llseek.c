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
#𐤀𐤌 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#𐤀𐤌 HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#𐤀𐤌 HAVE_UNISTD_H
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
#𐤀𐤌 SIZEOF_OFF_T >= 8
	أرجع lseek(fd, offset, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	أرجع lseek64(fd, offset, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	أرجع llseek(fd, offset, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t result;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	أرجع (retval == -1 ? retval : result);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#endif
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t offset, صحيح origin)
{
	ext2_loff_t result;
	ساكن صحيح do_compat = 0;

	𐤀𐤌 (do_compat)
		اذهب fallback;

	result = my_llseek (fd, offset, origin);
	𐤀𐤌 (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		𐤀𐤌 (offset < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			أرجع lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		أرجع -1;
	}
	أرجع result;
}

#وإلا /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t offset, صحيح origin)
{
#𐤀𐤌 defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	أرجع lseek64 (fd, offset, origin);
#وإلا
	𐤀𐤌 ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		أرجع -1;
	}
	أرجع lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


