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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
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
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <ctype.h>

#यदि defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#यदि defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,माप_प्रकार)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#यदि defined(__CYGWIN__) || defined (WIN32)
#अन्तर्भाव "windows.h"
#अन्तर्भाव "winioctl.h"

#यदि (_WIN32_WINNT >= 0x0500)
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
#अन्यथा
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम त्रुटि_सं;
	h = (HANDLE)_get_osfhandle(fd);
	यदि (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &retbytes, रिक्त)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} अन्यथा यदि (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, परिमाण(DISK_GEOMETRY),
				&gi, परिमाण(DISK_GEOMETRY),
				&retbytes, रिक्त)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} अन्यथा यदि (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#अन्यथा
	} अन्यथा {
		filesize = GetFileSize(h, रिक्त);
		यदि (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	बंद(fd);
	प्रत्यागम 0;
}

#अन्यथा

स्थिर पूर्णाङ्क valid_offset (पूर्णाङ्क fd, ext2_loff_t offset)
{
	वर्ण ch;

	यदि (ext2fs_llseek (fd, offset, 0) < 0)
		प्रत्यागम 0;
	यदि (पठ (fd, &ch, 1) < 1)
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
	अचिन्हित दीर्घ दीर्घ size64;
	ext2_loff_t ऊपरी, निचला;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम त्रुटि_सं;

#यदि defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	अचिन्हित पूर्णाङ्क size;

	यदि (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
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

		यदि ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		यदि (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			गच्छ out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		अचिन्हित दीर्घ	size;

		यदि (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			गच्छ out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		संरचना floppy_struct this_floppy;

		यदि (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
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

#यदि defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			यदि (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				गच्छ out;
			}
		}
#नोचेद्यदि defined(DIOCGDINFO)
		/* old disklabel interface */
		part = म_माप(file) - 1;
		यदि (part >= 0) {
			ch = file[part];
			यदि (है_अंक(ch))
				part = 0;
			अन्यथा यदि (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			अन्यथा
				part = -1;
		}
		यदि (part >= 0 && (ioctl(fd, DIOCGDINFO, (वर्ण *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			यदि (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				गच्छ out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		यदि (ext2fs_fstat(fd, &st) == 0)
			यदि (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				गच्छ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	निचला = 0;
	प्रति (ऊपरी = 1024; valid_offset(fd, ऊपरी); ऊपरी *= 2)
		निचला = ऊपरी;
	यावत् (निचला < ऊपरी - 1) {
		ध्रुव ext2_loff_t mid = (निचला + ऊपरी) / 2;

		यदि (valid_offset (fd, mid))
			निचला = mid;
		अन्यथा
			ऊपरी = mid;
	}
	valid_offset(fd, 0);
	size64 = निचला + 1;
	*retblocks = size64 / blocksize;
out:
	बंद(fd);
	प्रत्यागम rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	यदि (retval)
		प्रत्यागम retval;
	यदि (blocks >= (1ULL << 32))
		प्रत्यागम EFBIG;
	*retblocks = (blk_t) blocks;
	प्रत्यागम 0;
}

#ifdef DEBUG
पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	blk64_t	blocks;
	पूर्णाङ्क	retval;

	यदि (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		निकास(1);
	}
	म_लिखो("Device %s has %llu 1k blocks.\n", argv[1],
	       (अचिन्हित दीर्घ दीर्घ) locks);
	निकास(0);
}
#endif
