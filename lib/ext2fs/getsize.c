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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#অন্তর্ভাব <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#অন্তর্ভাব <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#অন্তর্ভাব <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#অন্তর্ভাব <sys/disk.h>
#endif
#ifdef __linux__
#অন্তর্ভাব <sys/utsname.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#অন্তর্ভাব <ctype.h>

#যদি defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#যদি defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#যদি defined(__CYGWIN__) || defined (WIN32)
#অন্তর্ভাব "windows.h"
#অন্তর্ভাব "winioctl.h"

#যদি (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ধ্রুব বর্ণ *file, পূর্ণাঙ্ক blocksize,
				  blk64_t *retblocks)
{
	পূর্ণাঙ্ক fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#অন্যথা
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		প্রত্যাগম errno;
	h = (HANDLE)_get_osfhandle(fd);
	যদি (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, পরিমাণ(PARTITION_INFORMATION),
			    &pi, পরিমাণ(PARTITION_INFORMATION),
			    &retbytes, রিক্ত)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} অন্যথা যদি (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, পরিমাণ(DISK_GEOMETRY),
				&gi, পরিমাণ(DISK_GEOMETRY),
				&retbytes, রিক্ত)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} অন্যথা যদি (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#অন্যথা
	} অন্যথা {
		filesize = GetFileSize(h, রিক্ত);
		যদি (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	প্রত্যাগম 0;
}

#অন্যথা

স্থির পূর্ণাঙ্ক valid_offset (পূর্ণাঙ্ক fd, ext2_loff_t offset)
{
	বর্ণ ch;

	যদি (ext2fs_llseek (fd, offset, 0) < 0)
		প্রত্যাগম 0;
	যদি (পঠ (fd, &ch, 1) < 1)
		প্রত্যাগম 0;
	প্রত্যাগম 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ধ্রুব বর্ণ *file, পূর্ণাঙ্ক blocksize,
				  blk64_t *retblocks)
{
	পূর্ণাঙ্ক	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t উপরের, নিচের;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		প্রত্যাগম errno;

#যদি defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned পূর্ণাঙ্ক size;

	যদি (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		গচ্ছ out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		পূর্ণাঙ্ক valid_blkgetsize64 = 1;
#ifdef __linux__
		সংরচনা utsname ut;

		যদি ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		যদি (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			গচ্ছ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		যদি (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			গচ্ছ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		সংরচনা floppy_struct this_floppy;

		যদি (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			গচ্ছ out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		পূর্ণাঙ্ক part;
		সংরচনা disklabel lab;
		সংরচনা partition *pp;
		বর্ণ ch;

#যদি defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			যদি (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				গচ্ছ out;
			}
		}
#নোচেদ্যদি defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		যদি (part >= 0) {
			ch = file[part];
			যদি (isdigit(ch))
				part = 0;
			অন্যথা যদি (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			অন্যথা
				part = -1;
		}
		যদি (part >= 0 && (ioctl(fd, DIOCGDINFO, (বর্ণ *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			যদি (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				গচ্ছ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		যদি (ext2fs_fstat(fd, &st) == 0)
			যদি (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				গচ্ছ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	নিচের = 0;
	প্রতি (উপরের = 1024; valid_offset(fd, উপরের); উপরের *= 2)
		নিচের = উপরের;
	যাবত্ (নিচের < উপরের - 1) {
		ধ্রুব ext2_loff_t mid = (নিচের + উপরের) / 2;

		যদি (valid_offset (fd, mid))
			নিচের = mid;
		অন্যথা
			উপরের = mid;
	}
	valid_offset(fd, 0);
	size64 = নিচের + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	প্রত্যাগম rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ধ্রুব বর্ণ *file, পূর্ণাঙ্ক blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	যদি (retval)
		প্রত্যাগম retval;
	যদি (blocks >= (1ULL << 32))
		প্রত্যাগম EFBIG;
	*retblocks = (blk_t) blocks;
	প্রত্যাগম 0;
}

#ifdef DEBUG
পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, বর্ণ **argv)
{
	blk64_t	blocks;
	পূর্ণাঙ্ক	retval;

	যদি (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	যদি (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
