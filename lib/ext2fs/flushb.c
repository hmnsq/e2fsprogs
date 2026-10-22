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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_SYS_IOCTL_H
#അന്തര്ഭാവ <sys/ioctl.h>
#endif
#എങ്കിൽ HAVE_SYS_MOUNT_H
#അന്തര്ഭാവ <sys/param.h>
#അന്തര്ഭാവ <sys/mount.h>		/* This may define BLKFLSBUF */
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

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
errcode_t ext2fs_sync_device(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#എങ്കിൽ defined(HAVE_FSYNC)
	എങ്കിൽ (fsync (fd) == -1)
		തിരികെ errno;
#endif

	എങ്കിൽ (flushb) {
		errcode_t	retval = 0;

#ifdef BLKFLSBUF
		എങ്കിൽ (ioctl (fd, BLKFLSBUF, 0) == 0)
			തിരികെ 0;
		retval = errno;
#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
#warning BLKFLSBUF not defined
#endif
#ifdef FDFLUSH
		/* In case this is a floppy */
		എങ്കിൽ (ioctl(fd, FDFLUSH, 0) == 0)
			തിരികെ 0;
		എങ്കിൽ (retval == 0)
			retval = errno;
#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
#warning FDFLUSH not defined
#endif
		തിരികെ retval;
	}
	തിരികെ 0;
}
