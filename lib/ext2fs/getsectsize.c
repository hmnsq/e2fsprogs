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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#અન્તર્ભાવ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#અન્તર્ભાવ <sys/ioctl.h>
#અન્તર્ભાવ <linux/fd.h>
#endif

#જો defined(__linux__) && defined(_IO)
#જો !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#જો !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(અચલ અક્ષર *file, પૂર્ણાંક *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	પાછા 0;
#નહીંતર // not _WIN32

	પૂર્ણાંક	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	જો (fd < 0)
		પાછા errno;

#ifdef BLKSSZGET
	જો (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		પાછા 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	જો (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		પાછા 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	પાછા 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
પૂર્ણાંક ext2fs_get_dio_alignment(પૂર્ણાંક fd)
{
	પૂર્ણાંક align = 0;

#ifdef BLKSSZGET
	જો (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	જો (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	જો (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	જો (align <= 0)
		align = getpagesize();
#endif
	જો (align <= 0)
		align = 4096;

	પાછા align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(અચલ અક્ષર *file, પૂર્ણાંક *sectsize)
{
#ifdef _WIN32

	પાછા ext2fs_get_device_sectsize(file, sectsize);

#નહીંતર // not _WIN32

	પૂર્ણાંક	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	જો (fd < 0)
		પાછા errno;

#ifdef BLKPBSZGET
	જો (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		પાછા 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	જો (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		પાછા 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	પાછા 0;

#endif // ifdef _WIN32
}
