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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_SYS_IOCTL_H
#समावेश <sys/ioctl.h>
#पूर्ण_अगर
#अगर HAVE_SYS_MOUNT_H
#समावेश <sys/param.h>
#समावेश <sys/mount.h>		/* This may define BLKFLSBUF */
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * For Linux, define BLKFLSBUF and FDFLUSH if necessary, since
 * not all portable header file does so for us.  This really should be
 * fixed in the glibc header files.  (Recent glibcs appear to define
 * BLKFLSBUF in sys/mount.h, but FDFLUSH still doesn't seem to be
 * defined anywhere portable.)  Until then....
 */
#अगर_घोषित __linux__
#अगर_अघोषित BLKFLSBUF
#घोषणा BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#पूर्ण_अगर
#अगर_अघोषित FDFLUSH
#घोषणा FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#पूर्ण_अगर
#पूर्ण_अगर

/*
 * This function will sync a device/file, and optionally attempt to
 * flush the buffer cache.  The latter is basically only useful for
 * system benchmarks and for torturing systems in burn-in tests.  :)
 */
errcode_t ext2fs_sync_device(पूर्णांक fd, पूर्णांक flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
#अगर defined(HAVE_FSYNC)
	अगर (fsync (fd) == -1)
		वापस त्रुटि_सं;
#पूर्ण_अगर

	अगर (flushb) {
		errcode_t	retval = 0;

#अगर_घोषित BLKFLSBUF
		अगर (ioctl (fd, BLKFLSBUF, 0) == 0)
			वापस 0;
		retval = त्रुटि_सं;
#या_अगर defined(__linux__)
#warning BLKFLSBUF not defined
#पूर्ण_अगर
#अगर_घोषित FDFLUSH
		/* In case this is a floppy */
		अगर (ioctl(fd, FDFLUSH, 0) == 0)
			वापस 0;
		अगर (retval == 0)
			retval = त्रुटि_सं;
#या_अगर defined(__linux__)
#warning FDFLUSH not defined
#पूर्ण_अगर
		वापस retval;
	}
	वापस 0;
}
