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

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل __MSDOS__
#ضمن <io.h>
#جيڪڏهن_پورو
#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2_io.h"

#جيڪڏهن_وصف_ٿيل __linux__

#ضمن <sys/syscall.h>

ساكن ext2_loff_t my_llseek (صحيح fd, ext2_loff_t وٿي, صحيح origin)
{
#إذا SIZEOF_OFF_T >= 8
	أرجع lseek(fd, وٿي, origin);
#وإلاإذا HAVE_LSEEK64_PROTOTYPE
	أرجع lseek64(fd, وٿي, origin);
#وإلاإذا HAVE_LLSEEK_PROTOTYPE
	أرجع llseek(fd, وٿي, origin);
#وإلاإذا defined(__NR__llseek)
	loff_t نتيجو;
	صحيح retval;
	retval = syscall(__NR__llseek, fd,
		(بنانشان ڊگهو)(وٿي >> 32),
		(بنانشان ڊگهو)(وٿي & 0xffffffff),
		&نتيجو, origin);
	أرجع (retval == -1 ? retval : نتيجو);
#وإلا
	errno = ENOSYS;
	أرجع -1;
#جيڪڏهن_پورو
}

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t وٿي, صحيح origin)
{
	ext2_loff_t نتيجو;
	ساكن صحيح do_compat = 0;

	إذا (do_compat)
		اذهب fallback;

	نتيجو = my_llseek (fd, وٿي, origin);
	إذا (نتيجو == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		إذا (وٿي < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			أرجع lseek(fd, (off_t) وٿي, origin);
		errno = EINVAL;
		أرجع -1;
	}
	أرجع نتيجو;
}

#وإلا /* !linux */

#جيڪڏهن_وصف_نه_ٿيل EINVAL
#وصف_ڪريو EINVAL EXT2_ET_INVALID_دليل
#جيڪڏهن_پورو

ext2_loff_t ext2fs_llseek (صحيح fd, ext2_loff_t وٿي, صحيح origin)
{
#إذا defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	أرجع lseek64 (fd, وٿي, origin);
#وإلا
	إذا ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (وٿي >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		أرجع -1;
	}
	أرجع lseek (fd, (off_t) وٿي, origin);
#جيڪڏهن_پورو
}

#جيڪڏهن_پورو 	/* linux */


