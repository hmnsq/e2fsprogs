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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
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

#यदि defined(__linux__) && defined(_IO)
#यदि !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#यदि !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(अचर वर्ण *file, पूर्णाङ्क *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	फर्काऊ 0;
#अन्यथा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		फर्काऊ त्रुटि_सं;

#ifdef BLKSSZGET
	यदि (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		बंद(fd);
		फर्काऊ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	यदि (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		फर्काऊ 0;
	}
#endif
	*sectsize = 0;
	बंद(fd);
	फर्काऊ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
पूर्णाङ्क ext2fs_get_dio_alignment(पूर्णाङ्क fd)
{
	पूर्णाङ्क align = 0;

#ifdef BLKSSZGET
	यदि (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	यदि (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	यदि (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	यदि (align <= 0)
		align = getpagesize();
#endif
	यदि (align <= 0)
		align = 4096;

	फर्काऊ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(अचर वर्ण *file, पूर्णाङ्क *sectsize)
{
#ifdef _WIN32

	फर्काऊ ext2fs_get_device_sectsize(file, sectsize);

#अन्यथा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		फर्काऊ त्रुटि_सं;

#ifdef BLKPBSZGET
	यदि (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		बंद(fd);
		फर्काऊ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	यदि (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		फर्काऊ 0;
	}
#endif
	*sectsize = 0;
	बंद(fd);
	फर्काऊ 0;

#endif // ifdef _WIN32
}
