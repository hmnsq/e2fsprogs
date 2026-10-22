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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#यदि_अन्तः
#यदि HAVE_SYS_MOUNT_H
#अन्तर्भाव <sys/param.h>
#अन्तर्भाव <sys/mount.h>		/* This may define BLKFLSBUF */
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#यदि_परिभाषितम् __linux__
#यदि_अपरिभाषितम् BLKFLSBUF
#परिभाषय BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#यदि_अन्तः
#यदि_अपरिभाषितम् FDFLUSH
#परिभाषय FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#यदि_अन्तः
#यदि_अन्तः

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(पूर्णाङ्क fd, पूर्णाङ्क flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#यदि defined(HAVE_FSYNC)
	यदि (fsync (fd) == -1)
		प्रत्यागम errno;
#यदि_अन्तः

	यदि (flushb) {
		errcode_t	retval = 0;

#यदि_परिभाषितम् BLKFLSBUF
		यदि (ioctl (fd, BLKFLSBUF, 0) == 0)
			प्रत्यागम 0;
		retval = errno;
#नोचेद्यदि defined(__linux__)
#warning BLKFLSBUF not defined
#यदि_अन्तः
#यदि_परिभाषितम् FDFLUSH
		/* In case this is a floppy */
		यदि (ioctl(fd, FDFLUSH, 0) == 0)
			प्रत्यागम 0;
		यदि (retval == 0)
			retval = errno;
#नोचेद्यदि defined(__linux__)
#warning FDFLUSH not defined
#यदि_अन्तः
		प्रत्यागम retval;
	}
	प्रत्यागम 0;
}
