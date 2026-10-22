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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
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

#ຖ້າ defined(__linux__) && defined(_IO)
#ຖ້າ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ຖ້າ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(const var_na *file, ຈຳນວນເຕັມ *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ສົ່ງຄືນ 0;
#ບໍ່ດັ່ງນັ້ນ // not _WIN32

	ຈຳນວນເຕັມ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ຖ້າ (fd < 0)
		ສົ່ງຄືນ errno;

#ifdef BLKSSZGET
	ຖ້າ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ສົ່ງຄືນ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ຖ້າ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ສົ່ງຄືນ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ສົ່ງຄືນ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ຈຳນວນເຕັມ ext2fs_get_dio_alignment(ຈຳນວນເຕັມ fd)
{
	ຈຳນວນເຕັມ align = 0;

#ifdef BLKSSZGET
	ຖ້າ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ຖ້າ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ຖ້າ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ຖ້າ (align <= 0)
		align = getpagesize();
#endif
	ຖ້າ (align <= 0)
		align = 4096;

	ສົ່ງຄືນ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(const var_na *file, ຈຳນວນເຕັມ *sectsize)
{
#ifdef _WIN32

	ສົ່ງຄືນ ext2fs_get_device_sectsize(file, sectsize);

#ບໍ່ດັ່ງນັ້ນ // not _WIN32

	ຈຳນວນເຕັມ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ຖ້າ (fd < 0)
		ສົ່ງຄືນ errno;

#ifdef BLKPBSZGET
	ຖ້າ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ສົ່ງຄືນ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ຖ້າ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ສົ່ງຄືນ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ສົ່ງຄືນ 0;

#endif // ifdef _WIN32
}
