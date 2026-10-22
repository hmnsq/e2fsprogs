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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
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
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <ctype.h>

#nếu defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#nếu defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#include "ext2_fs.h"
#include "ext2fs.h"

#nếu defined(__CYGWIN__) || defined (WIN32)
#include "windows.h"
#include "winioctl.h"

#nếu (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(const char *file, số_nguyên blocksize,
				  blk64_t *retblocks)
{
	số_nguyên fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#không_thì
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	nếu (fd < 0)
		trả_về errno;
	h = (HANDLE)_get_osfhandle(fd);
	nếu (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, sizeof(PARTITION_INFORMATION),
			    &pi, sizeof(PARTITION_INFORMATION),
			    &retbytes, NULL)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} không_thì nếu (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, sizeof(DISK_GEOMETRY),
				&gi, sizeof(DISK_GEOMETRY),
				&retbytes, NULL)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} không_thì nếu (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#không_thì
	} không_thì {
		filesize = GetFileSize(h, NULL);
		nếu (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	trả_về 0;
}

#không_thì

static số_nguyên valid_offset (số_nguyên fd, ext2_loff_t offset)
{
	char ch;

	nếu (ext2fs_llseek (fd, offset, 0) < 0)
		trả_về 0;
	nếu (read (fd, &ch, 1) < 1)
		trả_về 0;
	trả_về 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(const char *file, số_nguyên blocksize,
				  blk64_t *retblocks)
{
	số_nguyên	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t high, low;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	nếu (fd < 0)
		trả_về errno;

#nếu defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned số_nguyên size;

	nếu (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		goto out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		số_nguyên valid_blkgetsize64 = 1;
#ifdef __linux__
		struct utsname ut;

		nếu ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		nếu (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			goto out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		nếu (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		struct floppy_struct this_floppy;

		nếu (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		số_nguyên part;
		struct disklabel lab;
		struct partition *pp;
		char ch;

#nếu defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			nếu (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				goto out;
			}
		}
#elif defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		nếu (part >= 0) {
			ch = file[part];
			nếu (isdigit(ch))
				part = 0;
			không_thì nếu (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			không_thì
				part = -1;
		}
		nếu (part >= 0 && (ioctl(fd, DIOCGDINFO, (char *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			nếu (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				goto out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		nếu (ext2fs_fstat(fd, &st) == 0)
			nếu (S_ISREG(st.st_mode)) {
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
	cho (high = 1024; valid_offset(fd, high); high *= 2)
		low = high;
	trong_khi (low < high - 1) {
		const ext2_loff_t mid = (low + high) / 2;

		nếu (valid_offset (fd, mid))
			low = mid;
		không_thì
			high = mid;
	}
	valid_offset(fd, 0);
	size64 = low + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	trả_về rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(const char *file, số_nguyên blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	nếu (retval)
		trả_về retval;
	nếu (blocks >= (1ULL << 32))
		trả_về EFBIG;
	*retblocks = (blk_t) blocks;
	trả_về 0;
}

#ifdef DEBUG
số_nguyên main(số_nguyên argc, char **argv)
{
	blk64_t	blocks;
	số_nguyên	retval;

	nếu (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	nếu (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
