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
#ถ้า HAVE_UNISTD_H
#include <unistd.h>
#endif
#ถ้า HAVE_ERRNO_H
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

#ถ้า defined(__linux__) && defined(_IO)
#ถ้า !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ถ้า !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const var_na *file, จำนวนเต็ม *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	คืน 0;
#มิฉะนั้น // not _WIN32

	จำนวนเต็ม	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ถ้า (fd < 0)
		คืน errno;

#ifdef BLKSSZGET
	ถ้า (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		คืน 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ถ้า (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		คืน 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	คืน 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
จำนวนเต็ม ext2fs_get_dio_alignment(จำนวนเต็ม fd)
{
	จำนวนเต็ม align = 0;

#ifdef BLKSSZGET
	ถ้า (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ถ้า (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ถ้า (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ถ้า (align <= 0)
		align = getpagesize();
#endif
	ถ้า (align <= 0)
		align = 4096;

	คืน align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const var_na *file, จำนวนเต็ม *sectsize)
{
#ifdef _WIN32

	คืน ext2fs_get_device_sectsize(file, sectsize);

#มิฉะนั้น // not _WIN32

	จำนวนเต็ม	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ถ้า (fd < 0)
		คืน errno;

#ifdef BLKPBSZGET
	ถ้า (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		คืน 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ถ้า (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		คืน 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	คืน 0;

#endif // ifdef _WIN32
}
