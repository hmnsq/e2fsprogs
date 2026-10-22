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

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#अगर_घोषित HAVE_SYS_DISK_H
#समावेश <sys/disk.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_LINUX_FD_H
#समावेश <sys/ioctl.h>
#समावेश <linux/fd.h>
#पूर्ण_अगर

#अगर defined(__linux__) && defined(_IO)
#अगर !defined(BLKSSZGET)
#घोषणा BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#पूर्ण_अगर
#अगर !defined(BLKPBSZGET)
#घोषणा BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#पूर्ण_अगर
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(स्थिर अक्षर *file, पूर्णांक *sectsize)
{
#अगर_घोषित _WIN32
	*sectsize = 512; // just guessing
	वापस 0;
#अन्यथा // not _WIN32

	पूर्णांक	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	अगर (fd < 0)
		वापस त्रुटि_सं;

#अगर_घोषित BLKSSZGET
	अगर (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		बंद(fd);
		वापस 0;
	}
#पूर्ण_अगर
#अगर_घोषित DIOCGSECTORSIZE
	अगर (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		वापस 0;
	}
#पूर्ण_अगर
	*sectsize = 0;
	बंद(fd);
	वापस 0;

#पूर्ण_अगर // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
पूर्णांक ext2fs_get_dio_alignment(पूर्णांक fd)
{
	पूर्णांक align = 0;

#अगर_घोषित BLKSSZGET
	अगर (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#पूर्ण_अगर
#अगर_घोषित DIOCGSECTORSIZE
	अगर (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#पूर्ण_अगर

#अगर_घोषित _SC_PAGESIZE
	अगर (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#पूर्ण_अगर
#अगर_घोषित HAVE_GETPAGESIZE
	अगर (align <= 0)
		align = getpagesize();
#पूर्ण_अगर
	अगर (align <= 0)
		align = 4096;

	वापस align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(स्थिर अक्षर *file, पूर्णांक *sectsize)
{
#अगर_घोषित _WIN32

	वापस ext2fs_get_device_sectsize(file, sectsize);

#अन्यथा // not _WIN32

	पूर्णांक	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	अगर (fd < 0)
		वापस त्रुटि_सं;

#अगर_घोषित BLKPBSZGET
	अगर (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		बंद(fd);
		वापस 0;
	}
#पूर्ण_अगर
#अगर_घोषित DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	अगर (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		बंद(fd);
		वापस 0;
	}
#पूर्ण_अगर
	*sectsize = 0;
	बंद(fd);
	वापस 0;

#पूर्ण_अगर // ifdef _WIN32
}
