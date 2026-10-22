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

#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#यदि_परिभाषितम् HAVE_SYS_DISK_H
#अन्तर्भाव <sys/disk.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_LINUX_FD_H
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <linux/fd.h>
#यदि_अन्तः

#यदि defined(__linux__) && defined(_IO)
#यदि !defined(BLKSSZGET)
#परिभाषय BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#यदि_अन्तः
#यदि !defined(BLKPBSZGET)
#परिभाषय BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize)
{
#यदि_परिभाषितम् _WIN32
	*sectsize = 512; // just guessing
	प्रत्यागम 0;
#अन्यथा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम errno;

#यदि_परिभाषितम् BLKSSZGET
	यदि (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#यदि_अन्तः
#यदि_परिभाषितम् DIOCGSECTORSIZE
	यदि (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#यदि_अन्तः
	*sectsize = 0;
	close(fd);
	प्रत्यागम 0;

#यदि_अन्तः // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
पूर्णाङ्क ext2fs_get_dio_alignment(पूर्णाङ्क fd)
{
	पूर्णाङ्क align = 0;

#यदि_परिभाषितम् BLKSSZGET
	यदि (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#यदि_अन्तः
#यदि_परिभाषितम् DIOCGSECTORSIZE
	यदि (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#यदि_अन्तः

#यदि_परिभाषितम् _SC_PAGESIZE
	यदि (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_GETPAGESIZE
	यदि (align <= 0)
		align = getpagesize();
#यदि_अन्तः
	यदि (align <= 0)
		align = 4096;

	प्रत्यागम align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ध्रुव वर्ण *file, पूर्णाङ्क *sectsize)
{
#यदि_परिभाषितम् _WIN32

	प्रत्यागम ext2fs_get_device_sectsize(file, sectsize);

#अन्यथा // not _WIN32

	पूर्णाङ्क	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम errno;

#यदि_परिभाषितम् BLKPBSZGET
	यदि (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#यदि_अन्तः
#यदि_परिभाषितम् DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	यदि (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		प्रत्यागम 0;
	}
#यदि_अन्तः
	*sectsize = 0;
	close(fd);
	प्रत्यागम 0;

#यदि_अन्तः // ifdef _WIN32
}
