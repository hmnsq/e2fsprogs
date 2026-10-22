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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#ಅನ್ತರ್ಭಾವ <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#ಅನ್ತರ್ಭಾವ <sys/disk.h>
#endif
#ifdef __linux__
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#endif
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಅನ್ತರ್ಭಾವ <ctype.h>

#ಯದಿ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#ಯದಿ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ಯದಿ defined(__CYGWIN__) || defined (WIN32)
#ಅನ್ತರ್ಭಾವ "windows.h"
#ಅನ್ತರ್ಭಾವ "winioctl.h"

#ಯದಿ (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
				  blk64_t *retblocks)
{
	ಪೂರ್ಣಾಙ್ಕ fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#ಅನ್ಯಥಾ
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ errno;
	h = (HANDLE)_get_osfhandle(fd);
	ಯದಿ (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, ಪರಿಮಾಣ(PARTITION_INFORMATION),
			    &pi, ಪರಿಮಾಣ(PARTITION_INFORMATION),
			    &retbytes, ರಿಕ್ತ)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} ಅನ್ಯಥಾ ಯದಿ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ಪರಿಮಾಣ(DISK_GEOMETRY),
				&gi, ಪರಿಮಾಣ(DISK_GEOMETRY),
				&retbytes, ರಿಕ್ತ)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} ಅನ್ಯಥಾ ಯದಿ (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ಅನ್ಯಥಾ
	} ಅನ್ಯಥಾ {
		filesize = GetFileSize(h, ರಿಕ್ತ);
		ಯದಿ (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ಪ್ರತ್ಯಾಗಮ 0;
}

#ಅನ್ಯಥಾ

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ valid_offset (ಪೂರ್ಣಾಙ್ಕ fd, ext2_loff_t offset)
{
	ವರ್ಣ ch;

	ಯದಿ (ext2fs_llseek (fd, offset, 0) < 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (ಪಠ (fd, &ch, 1) < 1)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
				  blk64_t *retblocks)
{
	ಪೂರ್ಣಾಙ್ಕ	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t ಮೇಲಿನ, ಕೆಳಗಿನ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

#ಯದಿ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned ಪೂರ್ಣಾಙ್ಕ size;

	ಯದಿ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		ಗಚ್ಛ out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		ಪೂರ್ಣಾಙ್ಕ valid_blkgetsize64 = 1;
#ifdef __linux__
		ಸಂರಚನಾ utsname ut;

		ಯದಿ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		ಯದಿ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			ಗಚ್ಛ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		ಯದಿ (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			ಗಚ್ಛ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		ಸಂರಚನಾ floppy_struct this_floppy;

		ಯದಿ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			ಗಚ್ಛ out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		ಪೂರ್ಣಾಙ್ಕ part;
		ಸಂರಚನಾ disklabel lab;
		ಸಂರಚನಾ partition *pp;
		ವರ್ಣ ch;

#ಯದಿ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ಯದಿ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				ಗಚ್ಛ out;
			}
		}
#ನೋಚೇದ್ಯದಿ defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		ಯದಿ (part >= 0) {
			ch = file[part];
			ಯದಿ (isdigit(ch))
				part = 0;
			ಅನ್ಯಥಾ ಯದಿ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ಅನ್ಯಥಾ
				part = -1;
		}
		ಯದಿ (part >= 0 && (ioctl(fd, DIOCGDINFO, (ವರ್ಣ *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ಯದಿ (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				ಗಚ್ಛ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ಯದಿ (ext2fs_fstat(fd, &st) == 0)
			ಯದಿ (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				ಗಚ್ಛ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	ಕೆಳಗಿನ = 0;
	ಪ್ರತಿ (ಮೇಲಿನ = 1024; valid_offset(fd, ಮೇಲಿನ); ಮೇಲಿನ *= 2)
		ಕೆಳಗಿನ = ಮೇಲಿನ;
	ಯಾವತ್ (ಕೆಳಗಿನ < ಮೇಲಿನ - 1) {
		ಧ್ರುವ ext2_loff_t mid = (ಕೆಳಗಿನ + ಮೇಲಿನ) / 2;

		ಯದಿ (valid_offset (fd, mid))
			ಕೆಳಗಿನ = mid;
		ಅನ್ಯಥಾ
			ಮೇಲಿನ = mid;
	}
	valid_offset(fd, 0);
	size64 = ಕೆಳಗಿನ + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	ಪ್ರತ್ಯಾಗಮ rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (blocks >= (1ULL << 32))
		ಪ್ರತ್ಯಾಗಮ EFBIG;
	*retblocks = (blk_t) blocks;
	ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef DEBUG
ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	blk64_t	blocks;
	ಪೂರ್ಣಾಙ್ಕ	retval;

	ಯದಿ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	ಯದಿ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
