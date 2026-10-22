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
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#如果 HAVE_ERRNO_H
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

#如果 defined(__linux__) && defined(_IO)
#如果 !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#如果 !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const char *file, 整数 *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	返回 0;
#否则 // not _WIN32

	整数	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	如果 (fd < 0)
		返回 errno;

#ifdef BLKSSZGET
	如果 (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		返回 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	如果 (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		返回 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	返回 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
整数 ext2fs_get_dio_alignment(整数 fd)
{
	整数 align = 0;

#ifdef BLKSSZGET
	如果 (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	如果 (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	如果 (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	如果 (align <= 0)
		align = getpagesize();
#endif
	如果 (align <= 0)
		align = 4096;

	返回 align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const char *file, 整数 *sectsize)
{
#ifdef _WIN32

	返回 ext2fs_get_device_sectsize(file, sectsize);

#否则 // not _WIN32

	整数	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	如果 (fd < 0)
		返回 errno;

#ifdef BLKPBSZGET
	如果 (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		返回 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	如果 (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		返回 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	返回 0;

#endif // ifdef _WIN32
}
