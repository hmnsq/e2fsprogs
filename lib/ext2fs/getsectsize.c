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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#অন্তর্ভাৱ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#অন্তর্ভাৱ <sys/ioctl.h>
#অন্তর্ভাৱ <linux/fd.h>
#endif

#যদি defined(__linux__) && defined(_IO)
#যদি !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#যদি !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ঘূৰাই 0;
#অন্যথা // not _WIN32

	পূর্ণাঙ্ক	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		ঘূৰাই errno;

#ifdef BLKSSZGET
	যদি (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ঘূৰাই 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	যদি (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ঘূৰাই 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ঘূৰাই 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
পূর্ণাঙ্ক ext2fs_get_dio_alignment(পূর্ণাঙ্ক fd)
{
	পূর্ণাঙ্ক align = 0;

#ifdef BLKSSZGET
	যদি (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	যদি (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	যদি (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	যদি (align <= 0)
		align = getpagesize();
#endif
	যদি (align <= 0)
		align = 4096;

	ঘূৰাই align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *sectsize)
{
#ifdef _WIN32

	ঘূৰাই ext2fs_get_device_sectsize(file, sectsize);

#অন্যথা // not _WIN32

	পূর্ণাঙ্ক	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		ঘূৰাই errno;

#ifdef BLKPBSZGET
	যদি (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ঘূৰাই 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	যদি (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ঘূৰাই 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ঘূৰাই 0;

#endif // ifdef _WIN32
}
