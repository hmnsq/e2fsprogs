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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __MSDOS__
#ಅನ್ತರ್ಭಾವ <io.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_io.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__

#ಅನ್ತರ್ಭಾವ <sys/syscall.h>

ಸ್ಥಿರ ext2_loff_t my_llseek (ಪೂರ್ಣಾಂಕ fd, ext2_loff_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ origin)
{
#ಆದರೆ SIZEOF_OFF_T >= 8
	ಹಿಂತಿರುಗಿ lseek(fd, ಸ್ಥಾನಾಂತರ, origin);
#ಅಥವಾಆದರೆ HAVE_LSEEK64_PROTOTYPE
	ಹಿಂತಿರುಗಿ lseek64(fd, ಸ್ಥಾನಾಂತರ, origin);
#ಅಥವಾಆದರೆ HAVE_LLSEEK_PROTOTYPE
	ಹಿಂತಿರುಗಿ llseek(fd, ಸ್ಥಾನಾಂತರ, origin);
#ಅಥವಾಆದರೆ defined(__NR__llseek)
	loff_t ಫಲಿತಾಂಶ;
	ಪೂರ್ಣಾಂಕ retval;
	retval = syscall(__NR__llseek, fd,
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(ಸ್ಥಾನಾಂತರ >> 32),
		(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(ಸ್ಥಾನಾಂತರ & 0xffffffff),
		&ಫಲಿತಾಂಶ, origin);
	ಹಿಂತಿರುಗಿ (retval == -1 ? retval : ಫಲಿತಾಂಶ);
#ಇಲ್ಲದಿದ್ದರೆ
	errno = ENOSYS;
	ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ext2_loff_t ext2fs_llseek (ಪೂರ್ಣಾಂಕ fd, ext2_loff_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ origin)
{
	ext2_loff_t ಫಲಿತಾಂಶ;
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_compat = 0;

	ಆದರೆ (do_compat)
		ಹೋಗು fallback;

	ಫಲಿತಾಂಶ = my_llseek (fd, ಸ್ಥಾನಾಂತರ, origin);
	ಆದರೆ (ಫಲಿತಾಂಶ == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ಆದರೆ (ಸ್ಥಾನಾಂತರ < ((ext2_loff_t) 1 << ((ಗಾತ್ರ(off_t)*8) -1)))
			ಹಿಂತಿರುಗಿ lseek(fd, (off_t) ಸ್ಥಾನಾಂತರ, origin);
		errno = EINVAL;
		ಹಿಂತಿರುಗಿ -1;
	}
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !linux */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ EINVAL
#ವ್ಯಾಖ್ಯಾನಿಸು EINVAL EXT2_ET_INVALID_ವಾದ
#ಆದರೆ_ಅಂತ್ಯ

ext2_loff_t ext2fs_llseek (ಪೂರ್ಣಾಂಕ fd, ext2_loff_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ origin)
{
#ಆದರೆ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ಹಿಂತಿರುಗಿ lseek64 (fd, ಸ್ಥಾನಾಂತರ, origin);
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ ((ಗಾತ್ರ(off_t) < ಗಾತ್ರ(ext2_loff_t)) &&
	    (ಸ್ಥಾನಾಂತರ >= ((ext2_loff_t) 1 << ((ಗಾತ್ರ(off_t)*8) -1)))) {
		errno = EINVAL;
		ಹಿಂತಿರುಗಿ -1;
	}
	ಹಿಂತಿರುಗಿ lseek (fd, (off_t) ಸ್ಥಾನಾಂತರ, origin);
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ_ಅಂತ್ಯ 	/* linux */


