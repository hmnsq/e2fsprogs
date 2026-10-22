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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जइ HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#अन्तर्भाव <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#अन्तर्भाव <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#अन्तर्भाव <sys/disk.h>
#endif
#ifdef __linux__
#अन्तर्भाव <sys/utsname.h>
#endif
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <ctype.h>

#जइ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#जइ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#जइ defined(__CYGWIN__) || defined (WIN32)
#अन्तर्भाव "windows.h"
#अन्तर्भाव "winioctl.h"

#जइ (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				  blk64_t *retblocks)
{
	पूर्णाङ्क fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#अण्णहा
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जइ (fd < 0)
		प्रत्यागम errno;
	h = (HANDLE)_get_osfhandle(fd);
	जइ (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &retbytes, रिक्त)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} अण्णहा जइ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, परिमाण(DISK_GEOMETRY),
				&gi, परिमाण(DISK_GEOMETRY),
				&retbytes, रिक्त)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} अण्णहा जइ (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#अण्णहा
	} अण्णहा {
		filesize = GetFileSize(h, रिक्त);
		जइ (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	प्रत्यागम 0;
}

#अण्णहा

स्थिर पूर्णाङ्क valid_offset (पूर्णाङ्क fd, ext2_loff_t offset)
{
	वर्ण ch;

	जइ (ext2fs_llseek (fd, offset, 0) < 0)
		प्रत्यागम 0;
	जइ (पठ (fd, &ch, 1) < 1)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				  blk64_t *retblocks)
{
	पूर्णाङ्क	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t उच्चः, नीचः;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जइ (fd < 0)
		प्रत्यागम errno;

#जइ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned पूर्णाङ्क size;

	जइ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		गच्छ out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		पूर्णाङ्क valid_blkgetsize64 = 1;
#ifdef __linux__
		संरचना utsname ut;

		जइ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		जइ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			गच्छ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		जइ (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			गच्छ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		संरचना floppy_struct this_floppy;

		जइ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			गच्छ out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		पूर्णाङ्क part;
		संरचना disklabel lab;
		संरचना partition *pp;
		वर्ण ch;

#जइ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			जइ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				गच्छ out;
			}
		}
#नोचेद्यदि defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		जइ (part >= 0) {
			ch = file[part];
			जइ (isdigit(ch))
				part = 0;
			अण्णहा जइ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			अण्णहा
				part = -1;
		}
		जइ (part >= 0 && (ioctl(fd, DIOCGDINFO, (वर्ण *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			जइ (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				गच्छ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		जइ (ext2fs_fstat(fd, &st) == 0)
			जइ (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				गच्छ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	नीचः = 0;
	प्रति (उच्चः = 1024; valid_offset(fd, उच्चः); उच्चः *= 2)
		नीचः = उच्चः;
	जाव (नीचः < उच्चः - 1) {
		ध्रुव ext2_loff_t mid = (नीचः + उच्चः) / 2;

		जइ (valid_offset (fd, mid))
			नीचः = mid;
		अण्णहा
			उच्चः = mid;
	}
	valid_offset(fd, 0);
	size64 = नीचः + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	प्रत्यागम rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	जइ (retval)
		प्रत्यागम retval;
	जइ (blocks >= (1ULL << 32))
		प्रत्यागम EFBIG;
	*retblocks = (blk_t) blocks;
	प्रत्यागम 0;
}

#ifdef DEBUG
पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	blk64_t	blocks;
	पूर्णाङ्क	retval;

	जइ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	जइ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
