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

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#جيڪڏهن_وصف_ٿيل HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_LINUX_FD_H
#ضمن <sys/ioctl.h>
#ضمن <linux/fd.h>
#جيڪڏهن_پورو

#إذا defined(__linux__) && defined(_IO)
#إذا !defined(BLKSSZGET)
#وصف_ڪريو BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#جيڪڏهن_پورو
#إذا !defined(BLKPBSZGET)
#وصف_ڪريو BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#جيڪڏهن_وصف_ٿيل _WIN32
	*sectsize = 512; // just guessing
	أرجع 0;
#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#جيڪڏهن_وصف_ٿيل BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل DIOCGSECTORSIZE
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جيڪڏهن_پورو
	*sectsize = 0;
	close(fd);
	أرجع 0;

#جيڪڏهن_پورو // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#جيڪڏهن_وصف_ٿيل BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل DIOCGSECTORSIZE
	إذا (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _SC_PAGESIZE
	إذا (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_GETPAGESIZE
	إذا (align <= 0)
		align = getpagesize();
#جيڪڏهن_پورو
	إذا (align <= 0)
		align = 4096;

	أرجع align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#جيڪڏهن_وصف_ٿيل _WIN32

	أرجع ext2fs_get_device_sectsize(file, sectsize);

#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#جيڪڏهن_وصف_ٿيل BLKPBSZGET
	إذا (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#جيڪڏهن_پورو
	*sectsize = 0;
	close(fd);
	أرجع 0;

#جيڪڏهن_پورو // ifdef _WIN32
}
