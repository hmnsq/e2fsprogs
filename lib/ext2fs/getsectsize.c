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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_DISK_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/disk.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_FD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/fd.h>
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && defined(_IO)
#ਜੇ !defined(BLKSSZGET)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#ਜੇ_ਅੰਤ
#ਜੇ !defined(BLKPBSZGET)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *sectsize)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
	*sectsize = 512; // just guessing
	ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ // not _WIN32

	ਪੂਰਨ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ errno;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKSSZGET
	ਜੇ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DIOCGSECTORSIZE
	ਜੇ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
	*sectsize = 0;
	close(fd);
	ਵਾਪਸ 0;

#ਜੇ_ਅੰਤ // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ਪੂਰਨ ext2fs_get_dio_alignment(ਪੂਰਨ fd)
{
	ਪੂਰਨ align = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKSSZGET
	ਜੇ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DIOCGSECTORSIZE
	ਜੇ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _SC_PAGESIZE
	ਜੇ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETPAGESIZE
	ਜੇ (align <= 0)
		align = getpagesize();
#ਜੇ_ਅੰਤ
	ਜੇ (align <= 0)
		align = 4096;

	ਵਾਪਸ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *sectsize)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32

	ਵਾਪਸ ext2fs_get_device_sectsize(file, sectsize);

#ਨਹੀਂਤਾਂ // not _WIN32

	ਪੂਰਨ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ errno;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKPBSZGET
	ਜੇ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ਜੇ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ਵਾਪਸ 0;
	}
#ਜੇ_ਅੰਤ
	*sectsize = 0;
	close(fd);
	ਵਾਪਸ 0;

#ਜੇ_ਅੰਤ // ifdef _WIN32
}
