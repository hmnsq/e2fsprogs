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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_SYS_IOCTL_H
#அந்தர்பாவ <sys/ioctl.h>
#endif
#எனில் HAVE_SYS_MOUNT_H
#அந்தர்பாவ <sys/param.h>
#அந்தர்பாவ <sys/mount.h>		/* This may define BLKFLSBUF */
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

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
errcode_t ext2fs_sync_device(முழு fd, முழு flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#எனில் defined(HAVE_FSYNC)
	எனில் (fsync (fd) == -1)
		திரும்பு errno;
#endif

	எனில் (flushb) {
		errcode_t	retval = 0;

#ifdef BLKFLSBUF
		எனில் (ioctl (fd, BLKFLSBUF, 0) == 0)
			திரும்பு 0;
		retval = errno;
#அல்லதெனில் defined(__linux__)
#warning BLKFLSBUF not defined
#endif
#ifdef FDFLUSH
		/* In case this is a floppy */
		எனில் (ioctl(fd, FDFLUSH, 0) == 0)
			திரும்பு 0;
		எனில் (retval == 0)
			retval = errno;
#அல்லதெனில் defined(__linux__)
#warning FDFLUSH not defined
#endif
		திரும்பு retval;
	}
	திரும்பு 0;
}
