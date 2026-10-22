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
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#𐤀𐤌 HAVE_ERRNO_H
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

#𐤀𐤌 defined(__linux__) && defined(_IO)
#𐤀𐤌 !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#𐤀𐤌 !defined(BLKPBSZGET)
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
	أرجع 0;
#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	𐤀𐤌 (fd < 0)
		أرجع errno;

#ifdef BLKSSZGET
	𐤀𐤌 (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	𐤀𐤌 (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	أرجع 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحيح ext2fs_get_dio_alignment(صحيح fd)
{
	صحيح align = 0;

#ifdef BLKSSZGET
	𐤀𐤌 (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	𐤀𐤌 (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	𐤀𐤌 (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	𐤀𐤌 (align <= 0)
		align = getpagesize();
#endif
	𐤀𐤌 (align <= 0)
		align = 4096;

	أرجع align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحيح *sectsize)
{
#ifdef _WIN32

	أرجع ext2fs_get_device_sectsize(file, sectsize);

#وإلا // not _WIN32

	صحيح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	𐤀𐤌 (fd < 0)
		أرجع errno;

#ifdef BLKPBSZGET
	𐤀𐤌 (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	𐤀𐤌 (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		أرجع 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	أرجع 0;

#endif // ifdef _WIN32
}
