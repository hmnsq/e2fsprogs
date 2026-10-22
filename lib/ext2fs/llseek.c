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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#اگر_تعریف_شده __MSDOS__
#ضمن <io.h>
#پایان_اگر
#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2_io.h"

#اگر_تعریف_شده __linux__

#ضمن <sys/syscall.h>

ساكن ext2_loff_t my_llseek (صحيح fd, ext2_loff_t جابه_جایی, صحيح origin)
{
#إذا SIZEOF_OFF_T >= 8
	أرجع lseek(fd, جابه_جایی, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	أرجع lseek64(fd, جابه_جایی, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	أرجع llseek(fd, جابه_جایی, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t نتیجه;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(بیعلامت دراز)(جابه_جایی >> 32),
		(بیعلامت دراز)(جابه_جایی & 0xffffffff),
		&نتیجه, origin);
	أرجع (retval == -1 ? retval : نتیجه);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#پایان_اگر
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t جابه_جایی, صحيح origin)
{
	ext2_loff_t نتیجه;
	ساكن صحيح do_compat = 0;

	إذا (do_compat)
		اذهب fallback;

	نتیجه = my_llseek (fd, جابه_جایی, origin);
	إذا (نتیجه == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		إذا (جابه_جایی < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			أرجع lseek(fd, (off_t) جابه_جایی, origin);
		errno = EINVAL;
		أرجع -1;
	}
	أرجع نتیجه;
}

#وإلا /* !linux */

#اگر_تعریف_نشده EINVAL
#تعریف_کن EINVAL EXT2_ET_INVALID_آرگومان
#پایان_اگر

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t جابه_جایی, صحيح origin)
{
#إذا defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	أرجع lseek64 (fd, جابه_جایی, origin);
#وإلا
	إذا ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (جابه_جایی >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		أرجع -1;
	}
	أرجع lseek (fd, (off_t) جابه_جایی, origin);
#پایان_اگر
}

#پایان_اگر 	/* linux */


