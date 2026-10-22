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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جے_انت
#إذا HAVE_SYS_MOUNT_H
#ضمن <sys/param.h>
#ضمن <sys/mount.h>		/* This may define BLKFLSBUF */
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#جے_تعریف_شدہ __linux__
#جے_بنا_تعریف BLKFLSBUF
#تعریف_کرو BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#جے_انت
#جے_بنا_تعریف FDFLUSH
#تعریف_کرو FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#جے_انت
#جے_انت

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(صحيح fd, صحيح flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#إذا defined(HAVE_FSYNC)
	إذا (fsync (fd) == -1)
		أرجع errno;
#جے_انت

	إذا (flushb) {
		errcode_t	retval = 0;

#جے_تعریف_شدہ BLKFLSBUF
		إذا (ioctl (fd, BLKFLSBUF, 0) == 0)
			أرجع 0;
		retval = errno;
#وإلاإذا defined(__linux__)
#warning BLKFLSBUF not defined
#جے_انت
#جے_تعریف_شدہ FDFLUSH
		/* In case this is a floppy */
		إذا (ioctl(fd, FDFLUSH, 0) == 0)
			أرجع 0;
		إذا (retval == 0)
			retval = errno;
#وإلاإذا defined(__linux__)
#warning FDFLUSH not defined
#جے_انت
		أرجع retval;
	}
	أرجع 0;
}
