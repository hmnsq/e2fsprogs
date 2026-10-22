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
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#ပေႃး HAVE_ERRNO_H
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

#ပေႃး defined(__linux__) && defined(_IO)
#ပေႃး !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ပေႃး !defined(BLKPBSZGET)
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
	ပွၵ်ႈ 0;
#ပေႃးဢမ်ႇ // not _WIN32

	puur_naa_nka	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ပေႃး (fd < 0)
		ပွၵ်ႈ errno;

#ifdef BLKSSZGET
	ပေႃး (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ပွၵ်ႈ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ပေႃး (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ပွၵ်ႈ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ပွၵ်ႈ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
puur_naa_nka ext2fs_get_dio_alignment(puur_naa_nka fd)
{
	puur_naa_nka align = 0;

#ifdef BLKSSZGET
	ပေႃး (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ပေႃး (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ပေႃး (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ပေႃး (align <= 0)
		align = getpagesize();
#endif
	ပေႃး (align <= 0)
		align = 4096;

	ပွၵ်ႈ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const var_na *file, puur_naa_nka *sectsize)
{
#ifdef _WIN32

	ပွၵ်ႈ ext2fs_get_device_sectsize(file, sectsize);

#ပေႃးဢမ်ႇ // not _WIN32

	puur_naa_nka	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ပေႃး (fd < 0)
		ပွၵ်ႈ errno;

#ifdef BLKPBSZGET
	ပေႃး (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ပွၵ်ႈ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ပေႃး (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ပွၵ်ႈ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ပွၵ်ႈ 0;

#endif // ifdef _WIN32
}
