/*
 * getsize.c --- get the size of a partition.
 *
 * Copyright (C) 1995, 1995 Theodore Ts'o.
 * Copyright (C) 2003 VMware, Inc.
 *
 * Windows version of ext2fs_get_device_size by Chris Li, VMware.
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

#include "config.h"
#include <stdio.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ᤀᤡ᤺ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#include <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#include <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#include <sys/disk.h>
#endif
#ifdef __linux__
#include <sys/utsname.h>
#endif
#ᤀᤡ᤺ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <ctype.h>

#ᤀᤡ᤺ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#ᤀᤡ᤺ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#include "ext2_fs.h"
#include "ext2fs.h"

#ᤀᤡ᤺ defined(__CYGWIN__) || defined (WIN32)
#include "windows.h"
#include "winioctl.h"

#ᤀᤡ᤺ (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(const var_na *file, puur_naa_nka blocksize,
				  blk64_t *retblocks)
{
	puur_naa_nka fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#ᤀᤡ᤺ᤆᤡ
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ᤀᤡ᤺ (fd < 0)
		ᤀᤡᤴ errno;
	h = (HANDLE)_get_osfhandle(fd);
	ᤀᤡ᤺ (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, parimaa_na(PARTITION_INFORMATION),
			    &pi, parimaa_na(PARTITION_INFORMATION),
			    &retbytes, NULL)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, parimaa_na(DISK_GEOMETRY),
				&gi, parimaa_na(DISK_GEOMETRY),
				&retbytes, NULL)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ᤀᤡ᤺ᤆᤡ
	} ᤀᤡ᤺ᤆᤡ {
		filesize = GetFileSize(h, NULL);
		ᤀᤡ᤺ (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ᤀᤡᤴ 0;
}

#ᤀᤡ᤺ᤆᤡ

static puur_naa_nka valid_offset (puur_naa_nka fd, ext2_loff_t offset)
{
	var_na ch;

	ᤀᤡ᤺ (ext2fs_llseek (fd, offset, 0) < 0)
		ᤀᤡᤴ 0;
	ᤀᤡ᤺ (read (fd, &ch, 1) < 1)
		ᤀᤡᤴ 0;
	ᤀᤡᤴ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(const var_na *file, puur_naa_nka blocksize,
				  blk64_t *retblocks)
{
	puur_naa_nka	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t high, low;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ᤀᤡ᤺ (fd < 0)
		ᤀᤡᤴ errno;

#ᤀᤡ᤺ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned puur_naa_nka size;

	ᤀᤡ᤺ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		goto out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		puur_naa_nka valid_blkgetsize64 = 1;
#ifdef __linux__
		sa_nracanaa utsname ut;

		ᤀᤡ᤺ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		ᤀᤡ᤺ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			goto out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		ᤀᤡ᤺ (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		sa_nracanaa floppy_struct this_floppy;

		ᤀᤡ᤺ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		puur_naa_nka part;
		sa_nracanaa disklabel lab;
		sa_nracanaa partition *pp;
		var_na ch;

#ᤀᤡ᤺ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ᤀᤡ᤺ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				goto out;
			}
		}
#elif defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		ᤀᤡ᤺ (part >= 0) {
			ch = file[part];
			ᤀᤡ᤺ (isdigit(ch))
				part = 0;
			ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ᤀᤡ᤺ᤆᤡ
				part = -1;
		}
		ᤀᤡ᤺ (part >= 0 && (ioctl(fd, DIOCGDINFO, (var_na *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ᤀᤡ᤺ (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				goto out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ᤀᤡ᤺ (ext2fs_fstat(fd, &st) == 0)
			ᤀᤡ᤺ (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				goto out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	low = 0;
	for (high = 1024; valid_offset(fd, high); high *= 2)
		low = high;
	ᤀᤠ᤺ (low < high - 1) {
		const ext2_loff_t mid = (low + high) / 2;

		ᤀᤡ᤺ (valid_offset (fd, mid))
			low = mid;
		ᤀᤡ᤺ᤆᤡ
			high = mid;
	}
	valid_offset(fd, 0);
	size64 = low + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	ᤀᤡᤴ rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(const var_na *file, puur_naa_nka blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	ᤀᤡ᤺ (retval)
		ᤀᤡᤴ retval;
	ᤀᤡ᤺ (blocks >= (1ULL << 32))
		ᤀᤡᤴ EFBIG;
	*retblocks = (blk_t) blocks;
	ᤀᤡᤴ 0;
}

#ifdef DEBUG
puur_naa_nka main(puur_naa_nka argc, var_na **argv)
{
	blk64_t	blocks;
	puur_naa_nka	retval;

	ᤀᤡ᤺ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	ᤀᤡ᤺ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
