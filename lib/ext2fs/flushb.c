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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_IOCTL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_MOUNT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/param.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>		/* This may define BLKFLSBUF */
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BLKFLSBUF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ FDFLUSH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(ਪੂਰਨ fd, ਪੂਰਨ flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#ਜੇ defined(HAVE_FSYNC)
	ਜੇ (fsync (fd) == -1)
		ਵਾਪਸ errno;
#ਜੇ_ਅੰਤ

	ਜੇ (flushb) {
		errcode_t	retval = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKFLSBUF
		ਜੇ (ioctl (fd, BLKFLSBUF, 0) == 0)
			ਵਾਪਸ 0;
		retval = errno;
#ਜਾਂਜੇ defined(__linux__)
#warning BLKFLSBUF not defined
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FDFLUSH
		/* In case this is a floppy */
		ਜੇ (ioctl(fd, FDFLUSH, 0) == 0)
			ਵਾਪਸ 0;
		ਜੇ (retval == 0)
			retval = errno;
#ਜਾਂਜੇ defined(__linux__)
#warning FDFLUSH not defined
#ਜੇ_ਅੰਤ
		ਵਾਪਸ retval;
	}
	ਵਾਪਸ 0;
}
