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

#include "config.h"
#include <stdio.h>
#ပေႃး HAVE_ERRNO_H
#include <errno.h>
#endif
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#ပေႃး HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ပေႃး HAVE_SYS_MOUNT_H
#include <sys/param.h>
#include <sys/mount.h>		/* This may define BLKFLSBUF */
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

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
errcode_t ext2fs_sync_device(puur_naa_nka fd, puur_naa_nka flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#ပေႃး defined(HAVE_FSYNC)
	ပေႃး (fsync (fd) == -1)
		ပွၵ်ႈ errno;
#endif

	ပေႃး (flushb) {
		errcode_t	retval = 0;

#ifdef BLKFLSBUF
		ပေႃး (ioctl (fd, BLKFLSBUF, 0) == 0)
			ပွၵ်ႈ 0;
		retval = errno;
#elif defined(__linux__)
#warning BLKFLSBUF not defined
#endif
#ifdef FDFLUSH
		/* In case this is a floppy */
		ပေႃး (ioctl(fd, FDFLUSH, 0) == 0)
			ပွၵ်ႈ 0;
		ပေႃး (retval == 0)
			retval = errno;
#elif defined(__linux__)
#warning FDFLUSH not defined
#endif
		ပွၵ်ႈ retval;
	}
	ပွၵ်ႈ 0;
}
