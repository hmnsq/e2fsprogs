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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जइ HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#अन्तर्भाव <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <linux/fd.h>
#endif

#जइ defined(__linux__) && defined(_IO)
#जइ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#जइ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	प्रत्यागम 0;
#अण्णहा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जइ (fd < 0)
		प्रत्यागम errno;

#ifdef BLKSSZGET
	जइ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	जइ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	प्रत्यागम 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
पूर्णाङ्क ext2fs_get_dio_alignment(पूर्णाङ्क fd)
{
	पूर्णाङ्क align = 0;

#ifdef BLKSSZGET
	जइ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	जइ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	जइ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	जइ (align <= 0)
		align = getpagesize();
#endif
	जइ (align <= 0)
		align = 4096;

	प्रत्यागम align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize)
{
#ifdef _WIN32

	प्रत्यागम ext2fs_get_device_sectsize(file, sectsize);

#अण्णहा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जइ (fd < 0)
		प्रत्यागम errno;

#ifdef BLKPBSZGET
	जइ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	जइ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	प्रत्यागम 0;

#endif // ifdef _WIN32
}
