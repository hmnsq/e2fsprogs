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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#અન્તર્ભાવ <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#અન્તર્ભાવ <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#અન્તર્ભાવ <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#અન્તર્ભાવ <sys/disk.h>
#endif
#ifdef __linux__
#અન્તર્ભાવ <sys/utsname.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#અન્તર્ભાવ <ctype.h>

#જો defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#જો defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#જો defined(__CYGWIN__) || defined (WIN32)
#અન્તર્ભાવ "windows.h"
#અન્તર્ભાવ "winioctl.h"

#જો (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(અચલ અક્ષર *file, પૂર્ણાંક blocksize,
				  blk64_t *retblocks)
{
	પૂર્ણાંક fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#નહીંતર
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	જો (fd < 0)
		પાછા errno;
	h = (HANDLE)_get_osfhandle(fd);
	જો (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, કદ(PARTITION_INFORMATION),
			    &pi, કદ(PARTITION_INFORMATION),
			    &retbytes, ખાલી)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} નહીંતર જો (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, કદ(DISK_GEOMETRY),
				&gi, કદ(DISK_GEOMETRY),
				&retbytes, ખાલી)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} નહીંતર જો (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#નહીંતર
	} નહીંતર {
		filesize = GetFileSize(h, ખાલી);
		જો (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	પાછા 0;
}

#નહીંતર

સ્થિર પૂર્ણાંક valid_offset (પૂર્ણાંક fd, ext2_loff_t offset)
{
	અક્ષર ch;

	જો (ext2fs_llseek (fd, offset, 0) < 0)
		પાછા 0;
	જો (પઠ (fd, &ch, 1) < 1)
		પાછા 0;
	પાછા 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(અચલ અક્ષર *file, પૂર્ણાંક blocksize,
				  blk64_t *retblocks)
{
	પૂર્ણાંક	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t ઉપલું, નીચલું;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	જો (fd < 0)
		પાછા errno;

#જો defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned પૂર્ણાંક size;

	જો (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		જાઓ out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		પૂર્ણાંક valid_blkgetsize64 = 1;
#ifdef __linux__
		સંરચના utsname ut;

		જો ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		જો (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			જાઓ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		જો (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			જાઓ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		સંરચના floppy_struct this_floppy;

		જો (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			જાઓ out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		પૂર્ણાંક part;
		સંરચના disklabel lab;
		સંરચના partition *pp;
		અક્ષર ch;

#જો defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			જો (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				જાઓ out;
			}
		}
#અથવાજો defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		જો (part >= 0) {
			ch = file[part];
			જો (isdigit(ch))
				part = 0;
			નહીંતર જો (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			નહીંતર
				part = -1;
		}
		જો (part >= 0 && (ioctl(fd, DIOCGDINFO, (અક્ષર *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			જો (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				જાઓ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		જો (ext2fs_fstat(fd, &st) == 0)
			જો (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				જાઓ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	નીચલું = 0;
	માટે (ઉપલું = 1024; valid_offset(fd, ઉપલું); ઉપલું *= 2)
		નીચલું = ઉપલું;
	જ્યારે (નીચલું < ઉપલું - 1) {
		અચલ ext2_loff_t mid = (નીચલું + ઉપલું) / 2;

		જો (valid_offset (fd, mid))
			નીચલું = mid;
		નહીંતર
			ઉપલું = mid;
	}
	valid_offset(fd, 0);
	size64 = નીચલું + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	પાછા rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(અચલ અક્ષર *file, પૂર્ણાંક blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	જો (retval)
		પાછા retval;
	જો (blocks >= (1ULL << 32))
		પાછા EFBIG;
	*retblocks = (blk_t) blocks;
	પાછા 0;
}

#ifdef DEBUG
પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	blk64_t	blocks;
	પૂર્ણાંક	retval;

	જો (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	જો (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
