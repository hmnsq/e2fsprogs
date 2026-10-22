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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_DISK_H
#ಅನ್ತರ್ಭಾವ <sys/disk.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && defined(_IO)
#ಆದರೆ !defined(BLKSSZGET)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined(BLKPBSZGET)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *sectsize)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
	*sectsize = 512; // just guessing
	ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ // not _WIN32

	ಪೂರ್ಣಾಂಕ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ errno;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKSSZGET
	ಆದರೆ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DIOCGSECTORSIZE
	ಆದರೆ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
	*sectsize = 0;
	close(fd);
	ಹಿಂತಿರುಗಿ 0;

#ಆದರೆ_ಅಂತ್ಯ // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
ಪೂರ್ಣಾಂಕ ext2fs_get_dio_alignment(ಪೂರ್ಣಾಂಕ fd)
{
	ಪೂರ್ಣಾಂಕ align = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKSSZGET
	ಆದರೆ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DIOCGSECTORSIZE
	ಆದರೆ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _SC_PAGESIZE
	ಆದರೆ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETPAGESIZE
	ಆದರೆ (align <= 0)
		align = getpagesize();
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (align <= 0)
		align = 4096;

	ಹಿಂತಿರುಗಿ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *sectsize)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32

	ಹಿಂತಿರುಗಿ ext2fs_get_device_sectsize(file, sectsize);

#ಇಲ್ಲದಿದ್ದರೆ // not _WIN32

	ಪೂರ್ಣಾಂಕ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ errno;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKPBSZGET
	ಆದರೆ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ಆದರೆ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		ಹಿಂತಿರುಗಿ 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
	*sectsize = 0;
	close(fd);
	ಹಿಂತಿರುಗಿ 0;

#ಆದರೆ_ಅಂತ್ಯ // ifdef _WIN32
}
