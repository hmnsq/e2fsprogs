/*
 * flushb.c --- Hides system-dependent information for both syncing a
 * 	device to disk and to flush any buffers from disk cache.
 *
 * Copyright (C) 2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_MOUNT_H
#ಅನ್ತರ್ಭಾವ <sys/param.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>		/* This may define BLKFLSBUF */
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BLKFLSBUF
#ವ್ಯಾಖ್ಯಾನಿಸು BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FDFLUSH
#ವ್ಯಾಖ್ಯಾನಿಸು FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#ಆದರೆ defined(HAVE_FSYNC)
	ಆದರೆ (fsync (fd) == -1)
		ಹಿಂತಿರುಗಿ errno;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (flushb) {
		errcode_t	retval = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKFLSBUF
		ಆದರೆ (ioctl (fd, BLKFLSBUF, 0) == 0)
			ಹಿಂತಿರುಗಿ 0;
		retval = errno;
#ಅಥವಾಆದರೆ defined(__linux__)
#warning BLKFLSBUF not defined
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FDFLUSH
		/* In case this is a floppy */
		ಆದರೆ (ioctl(fd, FDFLUSH, 0) == 0)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (retval == 0)
			retval = errno;
#ಅಥವಾಆದರೆ defined(__linux__)
#warning FDFLUSH not defined
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ retval;
	}
	ಹಿಂತಿರುಗಿ 0;
}
