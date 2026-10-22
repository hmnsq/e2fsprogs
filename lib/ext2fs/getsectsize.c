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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#నిర్వచితమైతే HAVE_SYS_DISK_H
#అన్తర్భావ <sys/disk.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_LINUX_FD_H
#అన్తర్భావ <sys/ioctl.h>
#అన్తర్భావ <linux/fd.h>
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && defined(_IO)
#ఒకవేళ !defined(BLKSSZGET)
#నిర్వచించు BLKSSZGET  _IO(0x12,104)/* get block device sector size */
#అయితే_ముగింపు
#ఒకవేళ !defined(BLKPBSZGET)
#నిర్వచించు BLKPBSZGET _IO(0x12,123)/* get block physical sector size */
#అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * Returns the logical sector size of a device
 */
errcode_t ext2fs_get_device_sectsize(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *sectsize)
{
#నిర్వచితమైతే _WIN32
	*sectsize = 512; // just guessing
	తిరిగి 0;
#లేకపోతే // not _WIN32

	పూర్ణాంకం	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ఒకవేళ (fd < 0)
		తిరిగి errno;

#నిర్వచితమైతే BLKSSZGET
	ఒకవేళ (ioctl(fd, BLKSSZGET, sectsize) >= 0) {
		close(fd);
		తిరిగి 0;
	}
#అయితే_ముగింపు
#నిర్వచితమైతే DIOCGSECTORSIZE
	ఒకవేళ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		తిరిగి 0;
	}
#అయితే_ముగింపు
	*sectsize = 0;
	close(fd);
	తిరిగి 0;

#అయితే_ముగింపు // ifdef _WIN32
}

/*
 * Return desired alignment for direct I/O
 */
పూర్ణాంకం ext2fs_get_dio_alignment(పూర్ణాంకం fd)
{
	పూర్ణాంకం align = 0;

#నిర్వచితమైతే BLKSSZGET
	ఒకవేళ (ioctl(fd, BLKSSZGET, &align) < 0)
		align = 0;
#అయితే_ముగింపు
#నిర్వచితమైతే DIOCGSECTORSIZE
	ఒకవేళ (align <= 0 &&
	    ioctl(fd, DIOCGSECTORSIZE, &align) < 0)
		align = 0;
#అయితే_ముగింపు

#నిర్వచితమైతే _SC_PAGESIZE
	ఒకవేళ (align <= 0)
		align = sysconf(_SC_PAGESIZE);
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_GETPAGESIZE
	ఒకవేళ (align <= 0)
		align = getpagesize();
#అయితే_ముగింపు
	ఒకవేళ (align <= 0)
		align = 4096;

	తిరిగి align;
}

/*
 * Returns the physical sector size of a device
 */
errcode_t ext2fs_get_device_phys_sectsize(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *sectsize)
{
#నిర్వచితమైతే _WIN32

	తిరిగి ext2fs_get_device_sectsize(file, sectsize);

#లేకపోతే // not _WIN32

	పూర్ణాంకం	fd;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ఒకవేళ (fd < 0)
		తిరిగి errno;

#నిర్వచితమైతే BLKPBSZGET
	ఒకవేళ (ioctl(fd, BLKPBSZGET, sectsize) >= 0) {
		close(fd);
		తిరిగి 0;
	}
#అయితే_ముగింపు
#నిర్వచితమైతే DIOCGSECTORSIZE
	/* This isn't really the physical sector size, but FreeBSD
	 * doesn't seem to have this concept. */
	ఒకవేళ (ioctl(fd, DIOCGSECTORSIZE, sectsize) >= 0) {
		close(fd);
		తిరిగి 0;
	}
#అయితే_ముగింపు
	*sectsize = 0;
	close(fd);
	తిరిగి 0;

#అయితే_ముగింపు // ifdef _WIN32
}
