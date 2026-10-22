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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر_تعریف_شدہ __MSDOS__
#شامل <io.h>
#اگر_ختم
#شامل "et/com_err.h"
#شامل "ext2fs/ext2_io.h"

#اگر_تعریف_شدہ __linux__

#شامل <sys/syscall.h>

ساکن ext2_loff_t my_llseek (صحیح fd, ext2_loff_t انحراف, صحیح origin)
{
#اگر SIZEOF_OFF_T >= 8
	واپس lseek(fd, انحراف, origin);
#وگرنہ_اگر HAVE_LSEEK64_PROTOTYPE
	واپس lseek64(fd, انحراف, origin);
#وگرنہ_اگر HAVE_LLSEEK_PROTOTYPE
	واپس llseek(fd, انحراف, origin);
#وگرنہ_اگر defined(__NR__llseek)
	loff_t نتیجہ;
	صحیح retval;
	retval = syscall(__NR__llseek, fd,
		(غیرعلامتی طویل)(انحراف >> 32),
		(غیرعلامتی طویل)(انحراف & 0xffffffff),
		&نتیجہ, origin);
	واپس (retval == -1 ? retval : نتیجہ);
#ورنہ
	errno = ENOSYS;
	واپس -1;
#اگر_ختم
}

ext2_loff_t ext2fs_llseek (صحیح fd, ext2_loff_t انحراف, صحیح origin)
{
	ext2_loff_t نتیجہ;
	ساکن صحیح do_compat = 0;

	اگر (do_compat)
		جاؤ fallback;

	نتیجہ = my_llseek (fd, انحراف, origin);
	اگر (نتیجہ == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		اگر (انحراف < ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))
			واپس lseek(fd, (off_t) انحراف, origin);
		errno = EINVAL;
		واپس -1;
	}
	واپس نتیجہ;
}

#ورنہ /* !linux */

#اگر_غیر_تعریف_شدہ EINVAL
#تعریف_کرو EINVAL EXT2_ET_INVALID_دلیل
#اگر_ختم

ext2_loff_t ext2fs_llseek (صحیح fd, ext2_loff_t انحراف, صحیح origin)
{
#اگر defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	واپس lseek64 (fd, انحراف, origin);
#ورنہ
	اگر ((حجم(off_t) < حجم(ext2_loff_t)) &&
	    (انحراف >= ((ext2_loff_t) 1 << ((حجم(off_t)*8) -1)))) {
		errno = EINVAL;
		واپس -1;
	}
	واپس lseek (fd, (off_t) انحراف, origin);
#اگر_ختم
}

#اگر_ختم 	/* linux */


