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

#അന്തര്ഭാവ "config.h"
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#ifdef __MSDOS__
#അന്തര്ഭാവ <io.h>
#endif
#അന്തര്ഭാവ "et/com_err.h"
#അന്തര്ഭാവ "ext2fs/ext2_io.h"

#ifdef __linux__

#അന്തര്ഭാവ <sys/syscall.h>

സ്ഥിതം ext2_loff_t my_llseek (പൂർണ്ണസംഖ്യ fd, ext2_loff_t offset, പൂർണ്ണസംഖ്യ origin)
{
#എങ്കിൽ SIZEOF_OFF_T >= 8
	തിരികെ lseek(fd, offset, origin);
#അല്ലെങ്കിലെങ്കിൽ HAVE_LSEEK64_PROTOTYPE
	തിരികെ lseek64(fd, offset, origin);
#അല്ലെങ്കിലെങ്കിൽ HAVE_LLSEEK_PROTOTYPE
	തിരികെ llseek(fd, offset, origin);
#അല്ലെങ്കിലെങ്കിൽ defined(__NR__llseek)
	loff_t result;
	പൂർണ്ണസംഖ്യ retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	തിരികെ (retval == -1 ? retval : result);
#അല്ലെങ്കിൽ
	errno = ENOSYS;
	തിരികെ -1;
#endif
}

ext2_loff_t ext2fs_llseek (പൂർണ്ണസംഖ്യ fd, ext2_loff_t offset, പൂർണ്ണസംഖ്യ origin)
{
	ext2_loff_t result;
	സ്ഥിതം പൂർണ്ണസംഖ്യ do_compat = 0;

	എങ്കിൽ (do_compat)
		പോകുക fallback;

	result = my_llseek (fd, offset, origin);
	എങ്കിൽ (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		എങ്കിൽ (offset < ((ext2_loff_t) 1 << ((വലുപ്പം(off_t)*8) -1)))
			തിരികെ lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		തിരികെ -1;
	}
	തിരികെ result;
}

#അല്ലെങ്കിൽ /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (പൂർണ്ണസംഖ്യ fd, ext2_loff_t offset, പൂർണ്ണസംഖ്യ origin)
{
#എങ്കിൽ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	തിരികെ lseek64 (fd, offset, origin);
#അല്ലെങ്കിൽ
	എങ്കിൽ ((വലുപ്പം(off_t) < വലുപ്പം(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((വലുപ്പം(off_t)*8) -1)))) {
		errno = EINVAL;
		തിരികെ -1;
	}
	തിരികെ lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


