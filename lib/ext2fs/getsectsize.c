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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#ifdef HAVE_SYS_DISK_H
#അന്തര്ഭാവ <sys/disk.h>
#endif
#ifdef HAVE_LINUX_FD_H
#അന്തര്ഭാവ <sys/ioctl.h>
#അന്തര്ഭാവ <linux/fd.h>
#endif

#എങ്കിൽ defined(__linux__) && defined(_IO)
#എങ്കിൽ !defined(BLKSSZGET)
#define BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#endif
#എങ്കിൽ !defined(BLKPBSZGET)
#define BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#endif
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *sectsize)
{
#ifdef _WIN32
	*sectsize = 512; // just guessing
	തിരികെ 0;
#അല്ലെങ്കിൽ // not _WIN32

	പൂർണ്ണസംഖ്യ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ errno;

#ifdef BLKSSZGET
	എങ്കിൽ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		തിരികെ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	എങ്കിൽ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		തിരികെ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	തിരികെ 0;

#endif // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
പൂർണ്ണസംഖ്യ ext2fs_get_dio_alignment(പൂർണ്ണസംഖ്യ fd)
{
	പൂർണ്ണസംഖ്യ align = 0;

#ifdef BLKSSZGET
	എങ്കിൽ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#endif
#ifdef DIOCGSECTORSIZE
	എങ്കിൽ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#endif

#ifdef _SC_PAGESIZE
	എങ്കിൽ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#endif
#ifdef HAVE_GETPAGESIZE
	എങ്കിൽ (align <= 0)
		align = getpagesize();
#endif
	എങ്കിൽ (align <= 0)
		align = 4096;

	തിരികെ align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *sectsize)
{
#ifdef _WIN32

	തിരികെ ext2fs_get_device_sectsize(file, sectsize);

#അല്ലെങ്കിൽ // not _WIN32

	പൂർണ്ണസംഖ്യ	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ errno;

#ifdef BLKPBSZGET
	എങ്കിൽ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		തിരികെ 0;
	}
#endif
#ifdef DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	എങ്കിൽ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		തിരികെ 0;
	}
#endif
	*sectsize = 0;
	close(fd);
	തിരികെ 0;

#endif // ifdef _WIN32
}
