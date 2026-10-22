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

#અન્તર્ભાવ "config.h"
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#ifdef __MSDOS__
#અન્તર્ભાવ <io.h>
#endif
#અન્તર્ભાવ "et/com_err.h"
#અન્તર્ભાવ "ext2fs/ext2_io.h"

#ifdef __linux__

#અન્તર્ભાવ <sys/syscall.h>

સ્થિર ext2_loff_t my_llseek (પૂર્ણાંક fd, ext2_loff_t offset, પૂર્ણાંક origin)
{
#જો SIZEOF_OFF_T >= 8
	પાછા lseek(fd, offset, origin);
#અથવાજો HAVE_LSEEK64_PROTOTYPE
	પાછા lseek64(fd, offset, origin);
#અથવાજો HAVE_LLSEEK_PROTOTYPE
	પાછા llseek(fd, offset, origin);
#અથવાજો defined(__NR__llseek)
	loff_t result;
	પૂર્ણાંક retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	પાછા (retval == -1 ? retval : result);
#નહીંતર
	errno = ENOSYS;
	પાછા -1;
#endif
}

ext2_loff_t ext2fs_llseek (પૂર્ણાંક fd, ext2_loff_t offset, પૂર્ણાંક origin)
{
	ext2_loff_t result;
	સ્થિર પૂર્ણાંક do_compat = 0;

	જો (do_compat)
		જાઓ fallback;

	result = my_llseek (fd, offset, origin);
	જો (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		જો (offset < ((ext2_loff_t) 1 << ((કદ(off_t)*8) -1)))
			પાછા lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		પાછા -1;
	}
	પાછા result;
}

#નહીંતર /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (પૂર્ણાંક fd, ext2_loff_t offset, પૂર્ણાંક origin)
{
#જો defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	પાછા lseek64 (fd, offset, origin);
#નહીંતર
	જો ((કદ(off_t) < કદ(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((કદ(off_t)*8) -1)))) {
		errno = EINVAL;
		પાછા -1;
	}
	પાછા lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


