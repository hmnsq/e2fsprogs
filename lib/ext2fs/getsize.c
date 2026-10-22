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
#define HAVE_GET_FILE_আকার_EX 1
#endif

errcode_t ext2fs_get_device_size2(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
				  blk64_t *retblocks)
{
	পূর্ণসংখ্যা fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_আকার_EX
	LARGE_INTEGER filesize;
#অন্যথা
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		ফেরত errno;
	h = (HANDLE)_get_osfhandle(fd);
	যদি (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, আকার(PARTITION_INFORMATION),
			    &pi, আকার(PARTITION_INFORMATION),
			    &retbytes, ফাঁকা)) {

		*retblocks = pi.Partition_দৈর্ঘ্য.QuadPart / blocksize;

	} অন্যথা যদি (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, আকার(DISK_GEOMETRY),
				&gi, আকার(DISK_GEOMETRY),
				&retbytes, ফাঁকা)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_আকার_EX
	} অন্যথা যদি (Get_File_আকার_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#অন্যথা
	} অন্যথা {
		filesize = Get_File_আকার(h, ফাঁকা);
		যদি (INVALID_FILE_আকার != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ফেরত 0;
}

#অন্যথা

স্থির পূর্ণসংখ্যা valid_offset (পূর্ণসংখ্যা fd, ext2_loff_t offset)
{
	অক্ষর ch;

	যদি (ext2fs_llseek (fd, offset, 0) < 0)
		ফেরত 0;
	যদি (পঠ (fd, &ch, 1) < 1)
		ফেরত 0;
	ফেরত 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
				  blk64_t *retblocks)
{
	পূর্ণসংখ্যা	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t উপরের, নিচের;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	যদি (fd < 0)
		ফেরত errno;

#যদি defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned পূর্ণসংখ্যা আকার_2;

	যদি (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &আকার_2) >= 0) {
		*retblocks = size64 * আকার_2 / blocksize;
		যাও out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		পূর্ণসংখ্যা valid_blkgetsize64 = 1;
#ifdef __linux__
		কাঠামো utsname ut;

		যদি ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		যদি (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			যাও out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	আকার_2;

		যদি (ioctl(fd, BLKGETSIZE, &আকার_2) >= 0) {
			*retblocks = আকার_2 / (blocksize / 512);
			যাও out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		কাঠামো floppy_struct this_floppy;

		যদি (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.আকার_2 / (blocksize / 512);
			যাও out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		পূর্ণসংখ্যা part;
		কাঠামো disklabel lab;
		কাঠামো partition *pp;
		অক্ষর ch;

#যদি defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			যদি (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				যাও out;
			}
		}
#নতুবা_যদি defined(DIOCGDINFO)
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
		যদি (part >= 0 && (ioctl(fd, DIOCGDINFO, (অক্ষর *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			যদি (pp->p_আকার) {
				*retblocks = pp->p_আকার / (blocksize / 512);
				যাও out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		যদি (ext2fs_fstat(fd, &st) == 0)
			যদি (S_ISREG(st.st_mode)) {
				*retblocks = st.st_আকার / blocksize;
				যাও out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	নিচের = 0;
	জন্য (উপরের = 1024; valid_offset(fd, উপরের); উপরের *= 2)
		নিচের = উপরের;
	যখন (নিচের < উপরের - 1) {
		ধ্রুবক ext2_loff_t mid = (নিচের + উপরের) / 2;

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
	ফেরত rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_আকার(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	যদি (retval)
		ফেরত retval;
	যদি (blocks >= (1ULL << 32))
		ফেরত EFBIG;
	*retblocks = (blk_t) blocks;
	ফেরত 0;
}

#ifdef DEBUG
পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	blk64_t	blocks;
	পূর্ণসংখ্যা	retval;

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
