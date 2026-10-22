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

#ضمن "config.h"
#ضمن <stdio.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#אם HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#ضمن <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#endif
#ifdef __linux__
#ضمن <sys/utsname.h>
#endif
#אם HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ضمن <ctype.h>

#אם defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#אם defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#אם defined(__CYGWIN__) || defined (WIN32)
#ضمن "windows.h"
#ضمن "winioctl.h"

#אם (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#אחרת
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	אם (fd < 0)
		החזר errno;
	h = (HANDLE)_get_osfhandle(fd);
	אם (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, حجم(PARTITION_INFORMATION),
			    &pi, حجم(PARTITION_INFORMATION),
			    &retbytes, عدم)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} אחרת אם (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, عدم)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} אחרת אם (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#אחרת
	} אחרת {
		filesize = GetFileSize(h, عدم);
		אם (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	החזר 0;
}

#אחרת

ساكن صحيح valid_offset (صحيح fd, ext2_loff_t offset)
{
	حرف ch;

	אם (ext2fs_llseek (fd, offset, 0) < 0)
		החזר 0;
	אם (اقرأ (fd, &ch, 1) < 1)
		החזר 0;
	החזר 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t high, low;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	אם (fd < 0)
		החזר errno;

#אם defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned صحيح size;

	אם (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		اذهب out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		صحيح valid_blkgetsize64 = 1;
#ifdef __linux__
		بنية utsname ut;

		אם ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		אם (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			اذهب out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		אם (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			اذهب out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		بنية floppy_struct this_floppy;

		אם (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			اذهب out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		صحيح part;
		بنية disklabel lab;
		بنية partition *pp;
		حرف ch;

#אם defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			אם (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				اذهب out;
			}
		}
#وإلاإذا defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		אם (part >= 0) {
			ch = file[part];
			אם (isdigit(ch))
				part = 0;
			אחרת אם (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			אחרת
				part = -1;
		}
		אם (part >= 0 && (ioctl(fd, DIOCGDINFO, (حرف *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			אם (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				اذهب out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		אם (ext2fs_fstat(fd, &st) == 0)
			אם (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				اذهب out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	low = 0;
	עבור (high = 1024; valid_offset(fd, high); high *= 2)
		low = high;
	כלעוד (low < high - 1) {
		ثابت ext2_loff_t mid = (low + high) / 2;

		אם (valid_offset (fd, mid))
			low = mid;
		אחרת
			high = mid;
	}
	valid_offset(fd, 0);
	size64 = low + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	החזר rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	אם (retval)
		החזר retval;
	אם (blocks >= (1ULL << 32))
		החזר EFBIG;
	*retblocks = (blk_t) blocks;
	החזר 0;
}

#ifdef DEBUG
صحيح main(صحيح argc, حرف **argv)
{
	blk64_t	blocks;
	صحيح	retval;

	אם (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	אם (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
