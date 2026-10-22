/*
 * getsectsize.c --- get the sector size of a device.
 *
 * Copyright (C) 1995, 1995 Theodore Ts'o.
 * Copyright (C) 2003 VMware, Inc.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ضمن "config.h"
#ضمن <stdio.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#אם HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ضمن <sys/ioctl.h>
#ضمن <linux/fd.h>
#endif

#אם defined(__linux__) && defined(_IO)
#אם !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#אם !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	החזר 0;
#אחרת // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	אם (fd < 0)
		החזר errno;

#ifdef BLKSSZGET
	אם (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		החזר 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	אם (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		החזר 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	החזר 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#ifdef BLKSSZGET
	אם (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	אם (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	אם (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	אם (align <= 0)
		align = getpagesize();
#endif
	אם (align <= 0)
		align = 4096;

	החזר align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#ifdef _WIN32

	החזר ext2fs_get_device_sectsize(file, sectsize);

#אחרת // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	אם (fd < 0)
		החזר errno;

#ifdef BLKPBSZGET
	אם (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		החזר 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	אם (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		החזר 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	החזר 0;

#endif // ifdef _WIN32
}
