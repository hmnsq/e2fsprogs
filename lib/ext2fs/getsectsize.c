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

#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <fcntl.h>
#که_تعریف_شوی HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#د_که_پای
#که_تعریف_شوی HAVE_LINUX_FD_H
#ضمن <sys/ioctl.h>
#ضمن <linux/fd.h>
#د_که_پای

#إذا defined(__linux__) && defined(_IO)
#إذا !defined(BLKSSZGET)
#تعریف_کړه BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#د_که_پای
#إذا !defined(BLKPBSZGET)
#تعریف_کړه BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#د_که_پای
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#که_تعریف_شوی _WIN32
	*sectsize = 512; // just guessing
	أرجع 0;
#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#که_تعریف_شوی BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#د_که_پای
#که_تعریف_شوی DIOCGSECTORSIZE
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#د_که_پای
	*sectsize = 0;
	close(fd);
	أرجع 0;

#د_که_پای // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#که_تعریف_شوی BLKSSZGET
	إذا (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#د_که_پای
#که_تعریف_شوی DIOCGSECTORSIZE
	إذا (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#د_که_پای

#که_تعریف_شوی _SC_PAGESIZE
	إذا (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#د_که_پای
#که_تعریف_شوی HAVE_GETPAGESIZE
	إذا (align <= 0)
		align = getpagesize();
#د_که_پای
	إذا (align <= 0)
		align = 4096;

	أرجع align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#که_تعریف_شوی _WIN32

	أرجع ext2fs_get_device_sectsize(file, sectsize);

#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#که_تعریف_شوی BLKPBSZGET
	إذا (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#د_که_پای
#که_تعریف_شوی DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	إذا (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#د_که_پای
	*sectsize = 0;
	close(fd);
	أرجع 0;

#د_که_پای // ifdef _WIN32
}
