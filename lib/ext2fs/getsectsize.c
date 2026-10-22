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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
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

#nếu defined(__linux__) && defined(_IO)
#nếu !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#nếu !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const char *file, số_nguyên *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	trả_về 0;
#không_thì // not _WIN32

	số_nguyên	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	nếu (fd < 0)
		trả_về errno;

#ifdef BLKSSZGET
	nếu (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		trả_về 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	nếu (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		trả_về 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	trả_về 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
số_nguyên ext2fs_get_dio_alignment(số_nguyên fd)
{
	số_nguyên align = 0;

#ifdef BLKSSZGET
	nếu (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	nếu (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	nếu (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	nếu (align <= 0)
		align = getpagesize();
#endif
	nếu (align <= 0)
		align = 4096;

	trả_về align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const char *file, số_nguyên *sectsize)
{
#ifdef _WIN32

	trả_về ext2fs_get_device_sectsize(file, sectsize);

#không_thì // not _WIN32

	số_nguyên	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	nếu (fd < 0)
		trả_về errno;

#ifdef BLKPBSZGET
	nếu (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		trả_về 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	nếu (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		trả_về 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	trả_về 0;

#endif // ifdef _WIN32
}
