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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
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
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <ctype.h>

#जर defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#जर defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,माप_प्रकार)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#जर defined(__CYGWIN__) || defined (WIN32)
#अन्तर्भाव "windows.h"
#अन्तर्भाव "winioctl.h"

#जर (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(स्थिरांक अक्षर *file, पूर्णांक blocksize,
				  blk64_t *retblocks)
{
	पूर्णांक fd;
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
	जर (fd < 0)
		परत त्रुटि_सं;
	h = (HANDLE)_get_osfhandle(fd);
	जर (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, आकार(PARTITION_INFORMATION),
			    &pi, आकार(PARTITION_INFORMATION),
			    &retbytes, रिक्त)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} अन्यथा जर (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, आकार(DISK_GEOMETRY),
				&gi, आकार(DISK_GEOMETRY),
				&retbytes, रिक्त)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} अन्यथा जर (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#अन्यथा
	} अन्यथा {
		filesize = GetFileSize(h, रिक्त);
		जर (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	बंद(fd);
	परत 0;
}

#अन्यथा

स्थिर पूर्णांक valid_offset (पूर्णांक fd, ext2_loff_t offset)
{
	अक्षर ch;

	जर (ext2fs_llseek (fd, offset, 0) < 0)
		परत 0;
	जर (पठ (fd, &ch, 1) < 1)
		परत 0;
	परत 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(स्थिरांक अक्षर *file, पूर्णांक blocksize,
				  blk64_t *retblocks)
{
	पूर्णांक	fd, rc = 0;
	अचिन्हित दीर्घ दीर्घ size64;
	ext2_loff_t वरचा, खालचा;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	जर (fd < 0)
		परत त्रुटि_सं;

#जर defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	अचिन्हित पूर्णांक size;

	जर (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		जा out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		पूर्णांक valid_blkgetsize64 = 1;
#ifdef __linux__
		संरचना utsname ut;

		जर ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		जर (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			जा out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		अचिन्हित दीर्घ	size;

		जर (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			जा out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		संरचना floppy_struct this_floppy;

		जर (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			जा out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		पूर्णांक part;
		संरचना disklabel lab;
		संरचना partition *pp;
		अक्षर ch;

#जर defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			जर (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				जा out;
			}
		}
#किंवाजर defined(DIOCGDINFO)
		/* old disklabel interface */
		part = म_माप(file) - 1;
		जर (part >= 0) {
			ch = file[part];
			जर (है_अंक(ch))
				part = 0;
			अन्यथा जर (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			अन्यथा
				part = -1;
		}
		जर (part >= 0 && (ioctl(fd, DIOCGDINFO, (अक्षर *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			जर (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				जा out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		जर (ext2fs_fstat(fd, &st) == 0)
			जर (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				जा out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	खालचा = 0;
	साठी (वरचा = 1024; valid_offset(fd, वरचा); वरचा *= 2)
		खालचा = वरचा;
	पर्यंत (खालचा < वरचा - 1) {
		स्थिरांक ext2_loff_t mid = (खालचा + वरचा) / 2;

		जर (valid_offset (fd, mid))
			खालचा = mid;
		अन्यथा
			वरचा = mid;
	}
	valid_offset(fd, 0);
	size64 = खालचा + 1;
	*retblocks = size64 / blocksize;
out:
	बंद(fd);
	परत rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(स्थिरांक अक्षर *file, पूर्णांक blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	जर (retval)
		परत retval;
	जर (blocks >= (1ULL << 32))
		परत EFBIG;
	*retblocks = (blk_t) blocks;
	परत 0;
}

#ifdef DEBUG
पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	blk64_t	blocks;
	पूर्णांक	retval;

	जर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	जर (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		निकास(1);
	}
	म_लिखो("Device %s has %llu 1k blocks.\n", argv[1],
	       (अचिन्हित दीर्घ दीर्घ) locks);
	निकास(0);
}
#endif
