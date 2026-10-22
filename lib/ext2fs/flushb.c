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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_IOCTL_H
#అన్తర్భావ <sys/ioctl.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_MOUNT_H
#అన్తర్భావ <sys/param.h>
#అన్తర్భావ <sys/mount.h>		/* This may define BLKFLSBUF */
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#నిర్వచితమైతే __linux__
#నిర్వచితం_కాకపోతే BLKFLSBUF
#నిర్వచించు BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే FDFLUSH
#నిర్వచించు FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#అయితే_ముగింపు
#అయితే_ముగింపు

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(పూర్ణాంకం fd, పూర్ణాంకం flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#ఒకవేళ defined(HAVE_FSYNC)
	ఒకవేళ (fsync (fd) == -1)
		తిరిగి errno;
#అయితే_ముగింపు

	ఒకవేళ (flushb) {
		errcode_t	retval = 0;

#నిర్వచితమైతే BLKFLSBUF
		ఒకవేళ (ioctl (fd, BLKFLSBUF, 0) == 0)
			తిరిగి 0;
		retval = errno;
#లేదావేళ defined(__linux__)
#warning BLKFLSBUF not defined
#అయితే_ముగింపు
#నిర్వచితమైతే FDFLUSH
		/* In case this is a floppy */
		ఒకవేళ (ioctl(fd, FDFLUSH, 0) == 0)
			తిరిగి 0;
		ఒకవేళ (retval == 0)
			retval = errno;
#లేదావేళ defined(__linux__)
#warning FDFLUSH not defined
#అయితే_ముగింపు
		తిరిగి retval;
	}
	తిరిగి 0;
}
