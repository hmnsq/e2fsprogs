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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#ಅನ್ತರ್ಭಾವ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#endif

#ಯದಿ defined(__linux__) && defined(_IO)
#ಯದಿ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#ಯದಿ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ // not _WIN32

	ಪೂರ್ಣಾಙ್ಕ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

#ifdef BLKSSZGET
	ಯದಿ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	ಯದಿ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ಪ್ರತ್ಯಾಗಮ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_get_dio_alignment(ಪೂರ್ಣಾಙ್ಕ fd)
{
	ಪೂರ್ಣಾಙ್ಕ align = 0;

#ifdef BLKSSZGET
	ಯದಿ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	ಯದಿ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	ಯದಿ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	ಯದಿ (align <= 0)
		align = getpagesize();
#endif
	ಯದಿ (align <= 0)
		align = 4096;

	ಪ್ರತ್ಯಾಗಮ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *sectsize)
{
#ifdef _WIN32

	ಪ್ರತ್ಯಾಗಮ ext2fs_get_device_sectsize(file, sectsize);

#ಅನ್ಯಥಾ // not _WIN32

	ಪೂರ್ಣಾಙ್ಕ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

#ifdef BLKPBSZGET
	ಯದಿ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ಯದಿ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	ಪ್ರತ್ಯಾಗಮ 0;

#endif // ifdef _WIN32
}
