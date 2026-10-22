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

#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای

#ضمن "config.h"
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#که_تعریف_شوی __MSDOS__
#ضمن <io.h>
#د_که_پای
#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2_io.h"

#که_تعریف_شوی __linux__

#ضمن <sys/syscall.h>

ساكن ext2_loff_t my_llseek (صحيح fd, ext2_loff_t دننه_واټن, صحيح origin)
{
#إذا SIZEOF_OFF_T >= 8
	أرجع lseek(fd, دننه_واټن, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	أرجع lseek64(fd, دننه_واټن, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	أرجع llseek(fd, دننه_واټن, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t پایله;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(بینښه اوږد)(دننه_واټن >> 32),
		(بینښه اوږد)(دننه_واټن & 0xffffffff),
		&پایله, origin);
	أرجع (retval == -1 ? retval : پایله);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#د_که_پای
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t دننه_واټن, صحيح origin)
{
	ext2_loff_t پایله;
	ساكن صحيح do_compat = 0;

	إذا (do_compat)
		اذهب fallback;

	پایله = my_llseek (fd, دننه_واټن, origin);
	إذا (پایله == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		إذا (دننه_واټن < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			أرجع lseek(fd, (off_t) دننه_واټن, origin);
		errno = EINVAL;
		أرجع -1;
	}
	أرجع پایله;
}

#وإلا /* !linux */

#که_نه_تعریف_شوی EINVAL
#تعریف_کړه EINVAL EXT2_ET_INVALID_دلیل
#د_که_پای

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t دننه_واټن, صحيح origin)
{
#إذا defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	أرجع lseek64 (fd, دننه_واټن, origin);
#وإلا
	إذا ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (دننه_واټن >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		أرجع -1;
	}
	أرجع lseek (fd, (off_t) دننه_واټن, origin);
#د_که_پای
}

#د_که_پای 	/* linux */


