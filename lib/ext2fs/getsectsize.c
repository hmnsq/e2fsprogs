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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#ଅନ୍ତର୍ଭାଵ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#ଅନ୍ତର୍ଭାଵ <linux/fd.h>
#endif

#ଯଦି defined(__linux__) && defined(_IO)
#ଯଦି !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ଯଦି !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା // not _WIN32

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ errno;

#ifdef BLKSSZGET
	ଯଦି (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ଫେରନ୍ତୁ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ଯଦି (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ଫେରନ୍ତୁ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ଫେରନ୍ତୁ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_get_dio_alignment(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା align = 0;

#ifdef BLKSSZGET
	ଯଦି (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ଯଦି (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ଯଦି (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ଯଦି (align <= 0)
		align = getpagesize();
#endif
	ଯଦି (align <= 0)
		align = 4096;

	ଫେରନ୍ତୁ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sectsize)
{
#ifdef _WIN32

	ଫେରନ୍ତୁ ext2fs_get_device_sectsize(file, sectsize);

#ଅନ୍ୟଥା // not _WIN32

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ errno;

#ifdef BLKPBSZGET
	ଯଦି (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ଫେରନ୍ତୁ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ଯଦି (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ଫେରନ୍ତୁ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ଫେରନ୍ତୁ 0;

#endif // ifdef _WIN32
}
