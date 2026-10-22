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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#அந்தர்பாவ <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#அந்தர்பாவ <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#அந்தர்பாவ <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#அந்தர்பாவ <sys/disk.h>
#endif
#ifdef __linux__
#அந்தர்பாவ <sys/utsname.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#அந்தர்பாவ <ctype.h>

#எனில் defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#எனில் defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#எனில் defined(__CYGWIN__) || defined (WIN32)
#அந்தர்பாவ "windows.h"
#அந்தர்பாவ "winioctl.h"

#எனில் (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(மாறிலி எழுத்து *file, முழு blocksize,
				  blk64_t *retblocks)
{
	முழு fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#இல்லையெனில்
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	எனில் (fd < 0)
		திரும்பு errno;
	h = (HANDLE)_get_osfhandle(fd);
	எனில் (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, அளவு(PARTITION_INFORMATION),
			    &pi, அளவு(PARTITION_INFORMATION),
			    &retbytes, வெற்று)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} இல்லையெனில் எனில் (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, அளவு(DISK_GEOMETRY),
				&gi, அளவு(DISK_GEOMETRY),
				&retbytes, வெற்று)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} இல்லையெனில் எனில் (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#இல்லையெனில்
	} இல்லையெனில் {
		filesize = GetFileSize(h, வெற்று);
		எனில் (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	திரும்பு 0;
}

#இல்லையெனில்

நிலையான முழு valid_offset (முழு fd, ext2_loff_t offset)
{
	எழுத்து ch;

	எனில் (ext2fs_llseek (fd, offset, 0) < 0)
		திரும்பு 0;
	எனில் (பட (fd, &ch, 1) < 1)
		திரும்பு 0;
	திரும்பு 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(மாறிலி எழுத்து *file, முழு blocksize,
				  blk64_t *retblocks)
{
	முழு	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t உயர்வு, கீழ்;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	எனில் (fd < 0)
		திரும்பு errno;

#எனில் defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned முழு size;

	எனில் (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		செல் out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		முழு valid_blkgetsize64 = 1;
#ifdef __linux__
		கட்டமைப்பு utsname ut;

		எனில் ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		எனில் (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			செல் out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		எனில் (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			செல் out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		கட்டமைப்பு floppy_struct this_floppy;

		எனில் (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			செல் out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		முழு part;
		கட்டமைப்பு disklabel lab;
		கட்டமைப்பு partition *pp;
		எழுத்து ch;

#எனில் defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			எனில் (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				செல் out;
			}
		}
#அல்லதெனில் defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		எனில் (part >= 0) {
			ch = file[part];
			எனில் (isdigit(ch))
				part = 0;
			இல்லையெனில் எனில் (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			இல்லையெனில்
				part = -1;
		}
		எனில் (part >= 0 && (ioctl(fd, DIOCGDINFO, (எழுத்து *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			எனில் (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				செல் out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		எனில் (ext2fs_fstat(fd, &st) == 0)
			எனில் (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				செல் out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	கீழ் = 0;
	க்கு (உயர்வு = 1024; valid_offset(fd, உயர்வு); உயர்வு *= 2)
		கீழ் = உயர்வு;
	போது (கீழ் < உயர்வு - 1) {
		மாறிலி ext2_loff_t mid = (கீழ் + உயர்வு) / 2;

		எனில் (valid_offset (fd, mid))
			கீழ் = mid;
		இல்லையெனில்
			உயர்வு = mid;
	}
	valid_offset(fd, 0);
	size64 = கீழ் + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	திரும்பு rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(மாறிலி எழுத்து *file, முழு blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	எனில் (retval)
		திரும்பு retval;
	எனில் (blocks >= (1ULL << 32))
		திரும்பு EFBIG;
	*retblocks = (blk_t) blocks;
	திரும்பு 0;
}

#ifdef DEBUG
முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	blk64_t	blocks;
	முழு	retval;

	எனில் (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	எனில் (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
