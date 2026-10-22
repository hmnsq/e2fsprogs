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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#ଅନ୍ତର୍ଭାଵ <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#ଅନ୍ତର୍ଭାଵ <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#ଅନ୍ତର୍ଭାଵ <sys/disk.h>
#endif
#ifdef __linux__
#ଅନ୍ତର୍ଭାଵ <sys/utsname.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଅନ୍ତର୍ଭାଵ <ctype.h>

#ଯଦି defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#ଯଦି defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

#ଯଦି defined(__CYGWIN__) || defined (WIN32)
#ଅନ୍ତର୍ଭାଵ "windows.h"
#ଅନ୍ତର୍ଭାଵ "winioctl.h"

#ଯଦି (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize,
				  blk64_t *retblocks)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#ଅନ୍ୟଥା
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ errno;
	h = (HANDLE)_get_osfhandle(fd);
	ଯଦି (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, ଆକାର(PARTITION_INFORMATION),
			    &pi, ଆକାର(PARTITION_INFORMATION),
			    &retbytes, ଖାଲି)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} ଅନ୍ୟଥା ଯଦି (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ଆକାର(DISK_GEOMETRY),
				&gi, ଆକାର(DISK_GEOMETRY),
				&retbytes, ଖାଲି)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} ଅନ୍ୟଥା ଯଦି (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ଅନ୍ୟଥା
	} ଅନ୍ୟଥା {
		filesize = GetFileSize(h, ଖାଲି);
		ଯଦି (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ଫେରନ୍ତୁ 0;
}

#ଅନ୍ୟଥା

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid_offset (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2_loff_t offset)
{
	ଅକ୍ଷର ch;

	ଯଦି (ext2fs_llseek (fd, offset, 0) < 0)
		ଫେରନ୍ତୁ 0;
	ଯଦି (ପଠ (fd, &ch, 1) < 1)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize,
				  blk64_t *retblocks)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t ଉପର, ତଳ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ errno;

#ଯଦି defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;

	ଯଦି (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		ଯାଅ out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା valid_blkgetsize64 = 1;
#ifdef __linux__
		ସଂରଚନା utsname ut;

		ଯଦି ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		ଯଦି (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			ଯାଅ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		ଯଦି (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			ଯାଅ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		ସଂରଚନା floppy_struct this_floppy;

		ଯଦି (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			ଯାଅ out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା part;
		ସଂରଚନା disklabel lab;
		ସଂରଚନା partition *pp;
		ଅକ୍ଷର ch;

#ଯଦି defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ଯଦି (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				ଯାଅ out;
			}
		}
#ନଚେତଯଦି defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		ଯଦି (part >= 0) {
			ch = file[part];
			ଯଦି (isdigit(ch))
				part = 0;
			ଅନ୍ୟଥା ଯଦି (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ଅନ୍ୟଥା
				part = -1;
		}
		ଯଦି (part >= 0 && (ioctl(fd, DIOCGDINFO, (ଅକ୍ଷର *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ଯଦି (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				ଯାଅ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ଯଦି (ext2fs_fstat(fd, &st) == 0)
			ଯଦି (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				ଯାଅ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	ତଳ = 0;
	ପାଇଁ (ଉପର = 1024; valid_offset(fd, ଉପର); ଉପର *= 2)
		ତଳ = ଉପର;
	ଯେତେବେଳେ (ତଳ < ଉପର - 1) {
		ଅଚଳ ext2_loff_t mid = (ତଳ + ଉପର) / 2;

		ଯଦି (valid_offset (fd, mid))
			ତଳ = mid;
		ଅନ୍ୟଥା
			ଉପର = mid;
	}
	valid_offset(fd, 0);
	size64 = ତଳ + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	ଫେରନ୍ତୁ rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (blocks >= (1ULL << 32))
		ଫେରନ୍ତୁ EFBIG;
	*retblocks = (blk_t) blocks;
	ଫେରନ୍ତୁ 0;
}

#ifdef DEBUG
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	blk64_t	blocks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	retval;

	ଯଦି (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	ଯଦି (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
