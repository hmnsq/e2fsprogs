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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#اگر_تعریف_شدہ HAVE_SYS_DISK_H
#شامل <sys/disk.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_LINUX_FD_H
#شامل <sys/ioctl.h>
#شامل <linux/fd.h>
#اگر_ختم

#اگر defined(__linux__) && defined(_IO)
#اگر !defined(BLKSSZGET)
#تعریف_کرو BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#اگر_ختم
#اگر !defined(BLKPBSZGET)
#تعریف_کرو BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#اگر_ختم
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ثابت حرف *file, صحیح *sectsize)
{
#اگر_تعریف_شدہ _WIN32
	*sectsize = 512; // just guessing
	واپس 0;
#ورنہ // not _WIN32

	صحیح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		واپس errno;

#اگر_تعریف_شدہ BLKSSZGET
	اگر (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		واپس 0;
	}
#اگر_ختم
#اگر_تعریف_شدہ DIOCGSECTORSIZE
	اگر (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		واپس 0;
	}
#اگر_ختم
	*sectsize = 0;
	close(fd);
	واپس 0;

#اگر_ختم // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
صحیح ext2fs_get_dio_alignment(صحیح fd)
{
	صحیح align = 0;

#اگر_تعریف_شدہ BLKSSZGET
	اگر (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#اگر_ختم
#اگر_تعریف_شدہ DIOCGSECTORSIZE
	اگر (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#اگر_ختم

#اگر_تعریف_شدہ _SC_PAGESIZE
	اگر (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#اگر_ختم
#اگر_تعریف_شدہ HAVE_GETPAGESIZE
	اگر (align <= 0)
		align = getpagesize();
#اگر_ختم
	اگر (align <= 0)
		align = 4096;

	واپس align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ثابت حرف *file, صحیح *sectsize)
{
#اگر_تعریف_شدہ _WIN32

	واپس ext2fs_get_device_sectsize(file, sectsize);

#ورنہ // not _WIN32

	صحیح	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		واپس errno;

#اگر_تعریف_شدہ BLKPBSZGET
	اگر (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		واپس 0;
	}
#اگر_ختم
#اگر_تعریف_شدہ DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	اگر (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		واپس 0;
	}
#اگر_ختم
	*sectsize = 0;
	close(fd);
	واپس 0;

#اگر_ختم // ifdef _WIN32
}
