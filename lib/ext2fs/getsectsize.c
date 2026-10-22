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

#include "config.h"
#include <stdio.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#གལ་ཏེ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#include <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#include <sys/ioctl.h>
#include <linux/fd.h>
#endif

#གལ་ཏེ defined(__linux__) && defined(_IO)
#གལ་ཏེ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#གལ་ཏེ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const var_na *file, ཧྲིལ *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ཕྱིར 0;
#དེ་མིན // not _WIN32

	ཧྲིལ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	གལ་ཏེ (fd < 0)
		ཕྱིར errno;

#ifdef BLKSSZGET
	གལ་ཏེ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ཕྱིར 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	གལ་ཏེ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ཕྱིར 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ཕྱིར 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ཧྲིལ ext2fs_get_dio_alignment(ཧྲིལ fd)
{
	ཧྲིལ align = 0;

#ifdef BLKSSZGET
	གལ་ཏེ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	གལ་ཏེ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	གལ་ཏེ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	གལ་ཏེ (align <= 0)
		align = getpagesize();
#endif
	གལ་ཏེ (align <= 0)
		align = 4096;

	ཕྱིར align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const var_na *file, ཧྲིལ *sectsize)
{
#ifdef _WIN32

	ཕྱིར ext2fs_get_device_sectsize(file, sectsize);

#དེ་མིན // not _WIN32

	ཧྲིལ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	གལ་ཏེ (fd < 0)
		ཕྱིར errno;

#ifdef BLKPBSZGET
	གལ་ཏེ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ཕྱིར 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	གལ་ཏེ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ཕྱིར 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ཕྱིར 0;

#endif // ifdef _WIN32
}
