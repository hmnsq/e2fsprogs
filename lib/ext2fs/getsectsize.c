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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#அந்தர்பாவ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#அந்தர்பாவ <sys/ioctl.h>
#அந்தர்பாவ <linux/fd.h>
#endif

#எனில் defined(__linux__) && defined(_IO)
#எனில் !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#எனில் !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(மாறிலி எழுத்து *file, முழு *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	திரும்பு 0;
#இல்லையெனில் // not _WIN32

	முழு	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	எனில் (fd < 0)
		திரும்பு errno;

#ifdef BLKSSZGET
	எனில் (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		திரும்பு 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	எனில் (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		திரும்பு 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	திரும்பு 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
முழு ext2fs_get_dio_alignment(முழு fd)
{
	முழு align = 0;

#ifdef BLKSSZGET
	எனில் (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	எனில் (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	எனில் (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	எனில் (align <= 0)
		align = getpagesize();
#endif
	எனில் (align <= 0)
		align = 4096;

	திரும்பு align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(மாறிலி எழுத்து *file, முழு *sectsize)
{
#ifdef _WIN32

	திரும்பு ext2fs_get_device_sectsize(file, sectsize);

#இல்லையெனில் // not _WIN32

	முழு	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	எனில் (fd < 0)
		திரும்பு errno;

#ifdef BLKPBSZGET
	எனில் (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		திரும்பு 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	எனில் (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		திரும்பு 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	திரும்பு 0;

#endif // ifdef _WIN32
}
