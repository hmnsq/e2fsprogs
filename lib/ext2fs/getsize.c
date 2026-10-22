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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#شامل <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#شامل <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#شامل <sys/disk.h>
#endif
#ifdef __linux__
#شامل <sys/utsname.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#شامل <ctype.h>

#اگر defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#اگر defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#اگر defined(__CYGWIN__) || defined (WIN32)
#شامل "windows.h"
#شامل "winioctl.h"

#اگر (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ثابت نویسه *file, صحیح blocksize,
				  blk64_t *retblocks)
{
	صحیح fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#وگرنه
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		بازگشت errno;
	h = (HANDLE)_get_osfhandle(fd);
	اگر (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, اندازه(PARTITION_INFORMATION),
			    &pi, اندازه(PARTITION_INFORMATION),
			    &retbytes, پوچ)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} وگرنه اگر (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, اندازه(DISK_GEOMETRY),
				&gi, اندازه(DISK_GEOMETRY),
				&retbytes, پوچ)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} وگرنه اگر (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#وگرنه
	} وگرنه {
		filesize = GetFileSize(h, پوچ);
		اگر (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	بازگشت 0;
}

#وگرنه

ایستا صحیح valid_offset (صحیح fd, ext2_loff_t offset)
{
	نویسه ch;

	اگر (ext2fs_llseek (fd, offset, 0) < 0)
		بازگشت 0;
	اگر (اقرأ (fd, &ch, 1) < 1)
		بازگشت 0;
	بازگشت 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ثابت نویسه *file, صحیح blocksize,
				  blk64_t *retblocks)
{
	صحیح	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t بالا, کم;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		بازگشت errno;

#اگر defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned صحیح size;

	اگر (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		برو out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		صحیح valid_blkgetsize64 = 1;
#ifdef __linux__
		ساختار utsname ut;

		اگر ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		اگر (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			برو out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		اگر (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			برو out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		ساختار floppy_struct this_floppy;

		اگر (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			برو out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		صحیح part;
		ساختار disklabel lab;
		ساختار partition *pp;
		نویسه ch;

#اگر defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			اگر (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				برو out;
			}
		}
#وگرنهاگر defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		اگر (part >= 0) {
			ch = file[part];
			اگر (isdigit(ch))
				part = 0;
			وگرنه اگر (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			وگرنه
				part = -1;
		}
		اگر (part >= 0 && (ioctl(fd, DIOCGDINFO, (نویسه *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			اگر (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				برو out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		اگر (ext2fs_fstat(fd, &st) == 0)
			اگر (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				برو out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	کم = 0;
	برای (بالا = 1024; valid_offset(fd, بالا); بالا *= 2)
		کم = بالا;
	وقتی (کم < بالا - 1) {
		ثابت ext2_loff_t mid = (کم + بالا) / 2;

		اگر (valid_offset (fd, mid))
			کم = mid;
		وگرنه
			بالا = mid;
	}
	valid_offset(fd, 0);
	size64 = کم + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	بازگشت rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ثابت نویسه *file, صحیح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	اگر (retval)
		بازگشت retval;
	اگر (blocks >= (1ULL << 32))
		بازگشت EFBIG;
	*retblocks = (blk_t) blocks;
	بازگشت 0;
}

#ifdef DEBUG
صحیح اصلی(صحیح argc, نویسه **argv)
{
	blk64_t	blocks;
	صحیح	retval;

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
