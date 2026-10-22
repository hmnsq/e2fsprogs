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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_SYS_IOCTL_H
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#endif
#ଯଦି HAVE_SYS_MOUNT_H
#ଅନ୍ତର୍ଭାଵ <sys/param.h>
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>		/* This may define BLKFLSBUF */
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#ifdef __linux__
#ifndef BLKFLSBUF
#define BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#endif
#ifndef FDFLUSH
#define FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#endif
#endif

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#ଯଦି defined(HAVE_FSYNC)
	ଯଦି (fsync (fd) == -1)
		ଫେରନ୍ତୁ errno;
#endif

	ଯଦି (flushb) {
		errcode_t	retval = 0;

#ifdef BLKFLSBUF
		ଯଦି (ioctl (fd, BLKFLSBUF, 0) == 0)
			ଫେରନ୍ତୁ 0;
		retval = errno;
#ନଚେତଯଦି defined(__linux__)
#warning BLKFLSBUF not defined
#endif
#ifdef FDFLUSH
		/* In case this is a floppy */
		ଯଦି (ioctl(fd, FDFLUSH, 0) == 0)
			ଫେରନ୍ତୁ 0;
		ଯଦି (retval == 0)
			retval = errno;
#ନଚେତଯଦି defined(__linux__)
#warning FDFLUSH not defined
#endif
		ଫେରନ୍ତୁ retval;
	}
	ଫେରନ୍ତୁ 0;
}
