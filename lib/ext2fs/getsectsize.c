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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#اگر_تعریف_شده HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_LINUX_FD_H
#ضمن <sys/ioctl.h>
#ضمن <linux/fd.h>
#پایان_اگر

#إذا defined(__linux__) && defined(_IO)
#إذا !defined(BLKSSZGET)
#تعریف_کن BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#پایان_اگر
#إذا !defined(BLKPBSZGET)
#تعریف_کن BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#پایان_اگر
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#اگر_تعریف_شده _WIN32
	*sectsize = 512; // just guessing
	أرجع 0;
#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#اگر_تعریف_شده BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#پایان_اگر
#اگر_تعریف_شده DIOCGSECTORSIZE
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#پایان_اگر
	*sectsize = 0;
	close(fd);
	أرجع 0;

#پایان_اگر // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#اگر_تعریف_شده BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#پایان_اگر
#اگر_تعریف_شده DIOCGSECTORSIZE
	إذا (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#پایان_اگر

#اگر_تعریف_شده _SC_PAGESIZE
	إذا (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#پایان_اگر
#اگر_تعریف_شده HAVE_GETPAGESIZE
	إذا (align <= 0)
		align = getpagesize();
#پایان_اگر
	إذا (align <= 0)
		align = 4096;

	أرجع align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#اگر_تعریف_شده _WIN32

	أرجع ext2fs_get_device_sectsize(file, sectsize);

#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#اگر_تعریف_شده BLKPBSZGET
	إذا (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#پایان_اگر
#اگر_تعریف_شده DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#پایان_اگر
	*sectsize = 0;
	close(fd);
	أرجع 0;

#پایان_اگر // ifdef _WIN32
}
