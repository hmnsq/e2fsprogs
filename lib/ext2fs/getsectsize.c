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
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ꦪꦼꦤ꧀ HAVE_ERRNO_H
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

#ꦪꦼꦤ꧀ defined(__linux__) && defined(_IO)
#ꦪꦼꦤ꧀ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ꦪꦼꦤ꧀ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const var_na *file, puur_naa_nka *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ꦮꦭꦶ 0;
#ꦭꦶꦪ // not _WIN32

	puur_naa_nka	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ꦪꦼꦤ꧀ (fd < 0)
		ꦮꦭꦶ errno;

#ifdef BLKSSZGET
	ꦪꦼꦤ꧀ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ꦮꦭꦶ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ꦪꦼꦤ꧀ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ꦮꦭꦶ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ꦮꦭꦶ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
puur_naa_nka ext2fs_get_dio_alignment(puur_naa_nka fd)
{
	puur_naa_nka align = 0;

#ifdef BLKSSZGET
	ꦪꦼꦤ꧀ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ꦪꦼꦤ꧀ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ꦪꦼꦤ꧀ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ꦪꦼꦤ꧀ (align <= 0)
		align = getpagesize();
#endif
	ꦪꦼꦤ꧀ (align <= 0)
		align = 4096;

	ꦮꦭꦶ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const var_na *file, puur_naa_nka *sectsize)
{
#ifdef _WIN32

	ꦮꦭꦶ ext2fs_get_device_sectsize(file, sectsize);

#ꦭꦶꦪ // not _WIN32

	puur_naa_nka	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ꦪꦼꦤ꧀ (fd < 0)
		ꦮꦭꦶ errno;

#ifdef BLKPBSZGET
	ꦪꦼꦤ꧀ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ꦮꦭꦶ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ꦪꦼꦤ꧀ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ꦮꦭꦶ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ꦮꦭꦶ 0;

#endif // ifdef _WIN32
}
