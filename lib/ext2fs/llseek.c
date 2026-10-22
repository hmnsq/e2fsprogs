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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#جے_تعریف_شدہ __MSDOS__
#ضمن <io.h>
#جے_انت
#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2_io.h"

#جے_تعریف_شدہ __linux__

#ضمن <sys/syscall.h>

ساكن ext2_loff_t my_llseek (صحيح fd, ext2_loff_t وتھ, صحيح origin)
{
#إذا SIZEOF_OFF_T >= 8
	أرجع lseek(fd, وتھ, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	أرجع lseek64(fd, وتھ, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	أرجع llseek(fd, وتھ, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t نتیجہ;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(بنانشانی لما)(وتھ >> 32),
		(بنانشانی لما)(وتھ & 0xffffffff),
		&نتیجہ, origin);
	أرجع (retval == -1 ? retval : نتیجہ);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#جے_انت
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t وتھ, صحيح origin)
{
	ext2_loff_t نتیجہ;
	ساكن صحيح do_compat = 0;

	إذا (do_compat)
		اذهب fallback;

	نتیجہ = my_llseek (fd, وتھ, origin);
	إذا (نتیجہ == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		إذا (وتھ < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			أرجع lseek(fd, (off_t) وتھ, origin);
		errno = EINVAL;
		أرجع -1;
	}
	أرجع نتیجہ;
}

#وإلا /* !linux */

#جے_بنا_تعریف EINVAL
#تعریف_کرو EINVAL EXT2_ET_INVALID_دلیل
#جے_انت

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t وتھ, صحيح origin)
{
#إذا defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	أرجع lseek64 (fd, وتھ, origin);
#وإلا
	إذا ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (وتھ >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		أرجع -1;
	}
	أرجع lseek (fd, (off_t) وتھ, origin);
#جے_انت
}

#جے_انت 	/* linux */


