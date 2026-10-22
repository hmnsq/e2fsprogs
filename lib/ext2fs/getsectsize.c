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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
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

#जर defined(__linux__) && defined(_IO)
#जर !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#जर !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(स्थिरांक अक्षर *file, पूर्णांक *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	परत 0;
#अन्यथा // not _WIN32

	पूर्णांक	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जर (fd < 0)
		परत त्रुटि_सं;

#ifdef BLKSSZGET
	जर (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		बंद(fd);
		परत 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	जर (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		परत 0;
	}
#endif
	*sectsize = 0;
	बंद(fd);
	परत 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
पूर्णांक ext2fs_get_dio_alignment(पूर्णांक fd)
{
	पूर्णांक align = 0;

#ifdef BLKSSZGET
	जर (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	जर (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	जर (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	जर (align <= 0)
		align = getpagesize();
#endif
	जर (align <= 0)
		align = 4096;

	परत align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(स्थिरांक अक्षर *file, पूर्णांक *sectsize)
{
#ifdef _WIN32

	परत ext2fs_get_device_sectsize(file, sectsize);

#अन्यथा // not _WIN32

	पूर्णांक	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जर (fd < 0)
		परत त्रुटि_सं;

#ifdef BLKPBSZGET
	जर (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		बंद(fd);
		परत 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	जर (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		परत 0;
	}
#endif
	*sectsize = 0;
	बंद(fd);
	परत 0;

#endif // ifdef _WIN32
}
