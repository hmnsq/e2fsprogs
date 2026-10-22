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

#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#यदि_परिभाषितम् HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_LINUX_FD_H
#अन्तर्भाव <linux/fd.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_DISKLABEL_H
#अन्तर्भाव <sys/disklabel.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_DISK_H
#अन्तर्भाव <sys/disk.h>
#यदि_अन्तः
#यदि_परिभाषितम् __linux__
#अन्तर्भाव <sys/utsname.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#अन्तर्भाव <ctype.h>

#यदि defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#परिभाषय BLKGETSIZE _IO(0x12,96)	/* return device size */
#यदि_अन्तः

#यदि defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#परिभाषय BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#यदि_अन्तः

#यदि_परिभाषितम् APPLE_DARWIN
#परिभाषय BLKGETSIZE DKIOCGETBLOCKCOUNT32
#यदि_अन्तः /* APPLE_DARWIN */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#यदि defined(__CYGWIN__) || defined (WIN32)
#अन्तर्भाव "windows.h"
#अन्तर्भाव "winioctl.h"

#यदि (_WIN32_WINNT >= 0x0500)
#परिभाषय HAVE_GET_FILE_आकार_EX 1
#यदि_अन्तः

errcode_t ext2fs_get_device_size2(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				  blk64_t *retblocks)
{
	पूर्णाङ्क fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#यदि_परिभाषितम् HAVE_GET_FILE_आकार_EX
	LARGE_INTEGER filesize;
#अन्यथा
	DWORD filesize;
#यदि_अन्तः /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम errno;
	h = (HANDLE)_get_osfhandle(fd);
	यदि (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &pi, परिमाण(PARTITION_INFORMATION),
			    &retbytes, रिक्त)) {

		*retblocks = pi.Partition_दीर्घता.QuadPart / blocksize;

	} अन्यथा यदि (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, परिमाण(DISK_GEOMETRY),
				&gi, परिमाण(DISK_GEOMETRY),
				&retbytes, रिक्त)) {

		*retblocks = gi.अष्टकानि_प्रत्येकम्_Sector *
			     gi.Sectors_प्रत्येकम्_Track *
			     gi.Tracks_प्रत्येकम्_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#यदि_परिभाषितम् HAVE_GET_FILE_आकार_EX
	} अन्यथा यदि (Get_File_आकार_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#अन्यथा
	} अन्यथा {
		filesize = Get_File_आकार(h, रिक्त);
		यदि (INVALID_FILE_आकार != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#यदि_अन्तः /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	प्रत्यागम 0;
}

#अन्यथा

स्थिर पूर्णाङ्क valid_विस्थापनम् (पूर्णाङ्क fd, ext2_loff_t विस्थापनम्)
{
	वर्ण ch;

	यदि (ext2fs_llseek (fd, विस्थापनम्, 0) < 0)
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
	अचिह्नित दीर्घ दीर्घ size64;
	ext2_loff_t उच्चः, नीचः;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	यदि (fd < 0)
		प्रत्यागम errno;

#यदि defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	अचिह्नित पूर्णाङ्क आकार;

	यदि (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &आकार) >= 0) {
		*retblocks = size64 * आकार / blocksize;
		गच्छ बहिर्गमनम्;
	}
#यदि_अन्तः

#यदि_परिभाषितम् BLKGETSIZE64
	{
		पूर्णाङ्क valid_blkgetsize64 = 1;
#यदि_परिभाषितम् __linux__
		संरचना utsname ut;

		यदि ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#यदि_अन्तः
		यदि (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			गच्छ बहिर्गमनम्;
		}
	}
#यदि_अन्तः /* BLKGETSIZE64 */

#यदि_परिभाषितम् BLKGETSIZE
	{
		अचिह्नित दीर्घ	आकार;

		यदि (ioctl(fd, BLKGETSIZE, &आकार) >= 0) {
			*retblocks = आकार / (blocksize / 512);
			गच्छ बहिर्गमनम्;
		}
	}
#यदि_अन्तः

#यदि_परिभाषितम् FDGETPRM
	{
		संरचना floppy_struct this_floppy;

		यदि (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.आकार / (blocksize / 512);
			गच्छ बहिर्गमनम्;
		}
	}
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_SYS_DISKLABEL_H
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
				गच्छ बहिर्गमनम्;
			}
		}
#नोचेद्यदि defined(DIOCGDINFO)
		/* old disklabel interface */
		part = वाक्यदैर्घ्यम्(file) - 1;
		यदि (part >= 0) {
			ch = file[part];
			यदि (अङ्कः_वा(ch))
				part = 0;
			अन्यथा यदि (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			अन्यथा
				part = -1;
		}
		यदि (part >= 0 && (ioctl(fd, DIOCGDINFO, (वर्ण *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			यदि (pp->p_आकार) {
				*retblocks = pp->p_आकार / (blocksize / 512);
				गच्छ बहिर्गमनम्;
			}
		}
#यदि_अन्तः /* defined(DIOCG*) */
	}
#यदि_अन्तः /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		यदि (ext2fs_fstat(fd, &st) == 0)
			यदि (S_ISREG(st.st_बहुलकम्)) {
				*retblocks = st.st_आकार / blocksize;
				गच्छ बहिर्गमनम्;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	नीचः = 0;
	प्रति (उच्चः = 1024; valid_विस्थापनम्(fd, उच्चः); उच्चः *= 2)
		नीचः = उच्चः;
	यावत् (नीचः < उच्चः - 1) {
		ध्रुव ext2_loff_t mid = (नीचः + उच्चः) / 2;

		यदि (valid_विस्थापनम् (fd, mid))
			नीचः = mid;
		अन्यथा
			उच्चः = mid;
	}
	valid_विस्थापनम्(fd, 0);
	size64 = नीचः + 1;
	*retblocks = size64 / blocksize;
बहिर्गमनम्:
	close(fd);
	प्रत्यागम rc;
}

#यदि_अन्तः /* WIN32 */

errcode_t ext2fs_get_device_आकार(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	खण्डानि;

	retval = ext2fs_get_device_size2(file, blocksize, &खण्डानि);
	यदि (retval)
		प्रत्यागम retval;
	यदि (खण्डानि >= (1ULL << 32))
		प्रत्यागम EFBIG;
	*retblocks = (blk_t) खण्डानि;
	प्रत्यागम 0;
}

#यदि_परिभाषितम् DEBUG
पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	blk64_t	खण्डानि;
	पूर्णाङ्क	retval;

	यदि (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		निर्गच्छ(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &खण्डानि);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("Device %s has %llu 1k blocks.\n", argv[1],
	       (अचिह्नित दीर्घ दीर्घ) locks);
	निर्गच्छ(0);
}
#यदि_अन्तः
