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
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif
#ако HAVE_ERRNO_H
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
#ако HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <ctype.h>

#ако defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#ако defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#include "ext2_fs.h"
#include "ext2fs.h"

#ако defined(__CYGWIN__) || defined (WIN32)
#include "windows.h"
#include "winioctl.h"

#ако (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(const char *file, цяло blocksize,
				  blk64_t *retblocks)
{
	цяло fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#иначе
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ако (fd < 0)
		върни errno;
	h = (HANDLE)_get_osfhandle(fd);
	ако (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, sizeof(PARTITION_INFORMATION),
			    &pi, sizeof(PARTITION_INFORMATION),
			    &retbytes, NULL)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} иначе ако (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, sizeof(DISK_GEOMETRY),
				&gi, sizeof(DISK_GEOMETRY),
				&retbytes, NULL)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} иначе ако (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#иначе
	} иначе {
		filesize = GetFileSize(h, NULL);
		ако (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	върни 0;
}

#иначе

static цяло valid_offset (цяло fd, ext2_loff_t offset)
{
	char ch;

	ако (ext2fs_llseek (fd, offset, 0) < 0)
		върни 0;
	ако (read (fd, &ch, 1) < 1)
		върни 0;
	върни 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(const char *file, цяло blocksize,
				  blk64_t *retblocks)
{
	цяло	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t high, low;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ако (fd < 0)
		върни errno;

#ако defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned цяло size;

	ако (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		goto out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		цяло valid_blkgetsize64 = 1;
#ifdef __linux__
		struct utsname ut;

		ако ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		ако (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			goto out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		ако (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		struct floppy_struct this_floppy;

		ако (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			goto out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		цяло part;
		struct disklabel lab;
		struct partition *pp;
		char ch;

#ако defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ако (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				goto out;
			}
		}
#elif defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		ако (part >= 0) {
			ch = file[part];
			ако (isdigit(ch))
				part = 0;
			иначе ако (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			иначе
				part = -1;
		}
		ако (part >= 0 && (ioctl(fd, DIOCGDINFO, (char *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ако (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				goto out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ако (ext2fs_fstat(fd, &st) == 0)
			ако (S_ISREG(st.st_mode)) {
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
	за (high = 1024; valid_offset(fd, high); high *= 2)
		low = high;
	докато (low < high - 1) {
		const ext2_loff_t mid = (low + high) / 2;

		ако (valid_offset (fd, mid))
			low = mid;
		иначе
			high = mid;
	}
	valid_offset(fd, 0);
	size64 = low + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	върни rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(const char *file, цяло blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	ако (retval)
		върни retval;
	ако (blocks >= (1ULL << 32))
		върни EFBIG;
	*retblocks = (blk_t) blocks;
	върни 0;
}

#ifdef DEBUG
цяло main(цяло argc, char **argv)
{
	blk64_t	blocks;
	цяло	retval;

	ако (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	ако (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
