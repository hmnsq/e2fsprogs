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

#அந்தர்பாவ "config.h"
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#ifdef __MSDOS__
#அந்தர்பாவ <io.h>
#endif
#அந்தர்பாவ "et/com_err.h"
#அந்தர்பாவ "ext2fs/ext2_io.h"

#ifdef __linux__

#அந்தர்பாவ <sys/syscall.h>

நிலையான ext2_loff_t my_llseek (முழு fd, ext2_loff_t offset, முழு origin)
{
#எனில் SIZEOF_OFF_T >= 8
	திரும்பு lseek(fd, offset, origin);
#அல்லதெனில் HAVE_LSEEK64_PROTOTYPE
	திரும்பு lseek64(fd, offset, origin);
#அல்லதெனில் HAVE_LLSEEK_PROTOTYPE
	திரும்பு llseek(fd, offset, origin);
#அல்லதெனில் defined(__NR__llseek)
	loff_t result;
	முழு retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	திரும்பு (retval == -1 ? retval : result);
#இல்லையெனில்
	errno = ENOSYS;
	திரும்பு -1;
#endif
}

ext2_loff_t ext2fs_llseek (முழு fd, ext2_loff_t offset, முழு origin)
{
	ext2_loff_t result;
	நிலையான முழு do_compat = 0;

	எனில் (do_compat)
		செல் fallback;

	result = my_llseek (fd, offset, origin);
	எனில் (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		எனில் (offset < ((ext2_loff_t) 1 << ((அளவு(off_t)*8) -1)))
			திரும்பு lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		திரும்பு -1;
	}
	திரும்பு result;
}

#இல்லையெனில் /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (முழு fd, ext2_loff_t offset, முழு origin)
{
#எனில் defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	திரும்பு lseek64 (fd, offset, origin);
#இல்லையெனில்
	எனில் ((அளவு(off_t) < அளவு(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((அளவு(off_t)*8) -1)))) {
		errno = EINVAL;
		திரும்பு -1;
	}
	திரும்பு lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


