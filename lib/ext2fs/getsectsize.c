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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#شامل <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#شامل <sys/ioctl.h>
#شامل <linux/fd.h>
#endif

#اگر defined(__linux__) && defined(_IO)
#اگر !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#اگر !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت نویسه *file, صحیح *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	بازگشت 0;
#وگرنه // not _WIN32

	صحیح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		بازگشت errno;

#ifdef BLKSSZGET
	اگر (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		بازگشت 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	اگر (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		بازگشت 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	بازگشت 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحیح ext2fs_get_dio_alignment(صحیح fd)
{
	صحیح align = 0;

#ifdef BLKSSZGET
	اگر (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	اگر (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	اگر (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	اگر (align <= 0)
		align = getpagesize();
#endif
	اگر (align <= 0)
		align = 4096;

	بازگشت align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت نویسه *file, صحیح *sectsize)
{
#ifdef _WIN32

	بازگشت ext2fs_get_device_sectsize(file, sectsize);

#وگرنه // not _WIN32

	صحیح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		بازگشت errno;

#ifdef BLKPBSZGET
	اگر (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		بازگشت 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	اگر (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		بازگشت 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	بازگشت 0;

#endif // ifdef _WIN32
}
