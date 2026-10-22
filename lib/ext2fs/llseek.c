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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __MSDOS__
#ਅਨ੍ਤਰ੍ਭਾਵ <io.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "et/com_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2_io.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>

ਸਥਿਰ ext2_loff_t my_llseek (ਪੂਰਨ fd, ext2_loff_t ਵਿੱਥ, ਪੂਰਨ origin)
{
#ਜੇ SIZEOF_OFF_T >= 8
	ਵਾਪਸ lseek(fd, ਵਿੱਥ, origin);
#ਜਾਂਜੇ HAVE_LSEEK64_PROTOTYPE
	ਵਾਪਸ lseek64(fd, ਵਿੱਥ, origin);
#ਜਾਂਜੇ HAVE_LLSEEK_PROTOTYPE
	ਵਾਪਸ llseek(fd, ਵਿੱਥ, origin);
#ਜਾਂਜੇ defined(__NR__llseek)
	loff_t ਨਤੀਜਾ;
	ਪੂਰਨ retval;
	retval = syscall(__NR__llseek, fd,
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਵਿੱਥ >> 32),
		(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਵਿੱਥ & 0xffffffff),
		&ਨਤੀਜਾ, origin);
	ਵਾਪਸ (retval == -1 ? retval : ਨਤੀਜਾ);
#ਨਹੀਂਤਾਂ
	errno = ENOSYS;
	ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ext2_loff_t ext2fs_llseek (ਪੂਰਨ fd, ext2_loff_t ਵਿੱਥ, ਪੂਰਨ origin)
{
	ext2_loff_t ਨਤੀਜਾ;
	ਸਥਿਰ ਪੂਰਨ do_compat = 0;

	ਜੇ (do_compat)
		ਜਾਓ fallback;

	ਨਤੀਜਾ = my_llseek (fd, ਵਿੱਥ, origin);
	ਜੇ (ਨਤੀਜਾ == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ਜੇ (ਵਿੱਥ < ((ext2_loff_t) 1 << ((ਆਕਾਰ(off_t)*8) -1)))
			ਵਾਪਸ lseek(fd, (off_t) ਵਿੱਥ, origin);
		errno = EINVAL;
		ਵਾਪਸ -1;
	}
	ਵਾਪਸ ਨਤੀਜਾ;
}

#ਨਹੀਂਤਾਂ /* !linux */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EINVAL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EINVAL EXT2_ET_INVALID_ਦਲੀਲ
#ਜੇ_ਅੰਤ

ext2_loff_t ext2fs_llseek (ਪੂਰਨ fd, ext2_loff_t ਵਿੱਥ, ਪੂਰਨ origin)
{
#ਜੇ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ਵਾਪਸ lseek64 (fd, ਵਿੱਥ, origin);
#ਨਹੀਂਤਾਂ
	ਜੇ ((ਆਕਾਰ(off_t) < ਆਕਾਰ(ext2_loff_t)) &&
	    (ਵਿੱਥ >= ((ext2_loff_t) 1 << ((ਆਕਾਰ(off_t)*8) -1)))) {
		errno = EINVAL;
		ਵਾਪਸ -1;
	}
	ਵਾਪਸ lseek (fd, (off_t) ਵਿੱਥ, origin);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅੰਤ 	/* linux */


