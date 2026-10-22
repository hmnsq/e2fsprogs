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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#جے_تعریف_شدہ HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#جے_انت
#جے_تعریف_شدہ HAVE_LINUX_FD_H
#ضمن <sys/ioctl.h>
#ضمن <linux/fd.h>
#جے_انت

#إذا defined(__linux__) && defined(_IO)
#إذا !defined(BLKSSZGET)
#تعریف_کرو BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#جے_انت
#إذا !defined(BLKPBSZGET)
#تعریف_کرو BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#جے_انت
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#جے_تعریف_شدہ _WIN32
	*sectsize = 512; // just guessing
	أرجع 0;
#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#جے_تعریف_شدہ BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جے_انت
#جے_تعریف_شدہ DIOCGSECTORSIZE
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جے_انت
	*sectsize = 0;
	close(fd);
	أرجع 0;

#جے_انت // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#جے_تعریف_شدہ BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#جے_انت
#جے_تعریف_شدہ DIOCGSECTORSIZE
	إذا (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#جے_انت

#جے_تعریف_شدہ _SC_PAGESIZE
	إذا (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#جے_انت
#جے_تعریف_شدہ HAVE_GETPAGESIZE
	إذا (align <= 0)
		align = getpagesize();
#جے_انت
	إذا (align <= 0)
		align = 4096;

	أرجع align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#جے_تعریف_شدہ _WIN32

	أرجع ext2fs_get_device_sectsize(file, sectsize);

#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#جے_تعریف_شدہ BLKPBSZGET
	إذا (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جے_انت
#جے_تعریف_شدہ DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جے_انت
	*sectsize = 0;
	close(fd);
	أرجع 0;

#جے_انت // ifdef _WIN32
}
