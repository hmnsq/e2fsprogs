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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#నిర్వచితమైతే HAVE_SYS_IOCTL_H
#అన్తర్భావ <sys/ioctl.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_LINUX_FD_H
#అన్తర్భావ <linux/fd.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_DISKLABEL_H
#అన్తర్భావ <sys/disklabel.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_DISK_H
#అన్తర్భావ <sys/disk.h>
#అయితే_ముగింపు
#నిర్వచితమైతే __linux__
#అన్తర్భావ <sys/utsname.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#అన్తర్భావ <ctype.h>

#ఒకవేళ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#నిర్వచించు BLKGETSIZE _IO(0x12,96)	/* return device size */
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#నిర్వచించు BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#అయితే_ముగింపు

#నిర్వచితమైతే APPLE_DARWIN
#నిర్వచించు BLKGETSIZE DKIOCGETBLOCKCOUNT32
#అయితే_ముగింపు /* APPLE_DARWIN */

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#ఒకవేళ defined(__CYGWIN__) || defined (WIN32)
#అన్తర్భావ "windows.h"
#అన్తర్భావ "winioctl.h"

#ఒకవేళ (_WIN32_WINNT >= 0x0500)
#నిర్వచించు HAVE_GET_FILE_కొలత_EX 1
#అయితే_ముగింపు

errcode_t ext2fs_get_device_size2(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
				  blk64_t *retblocks)
{
	పూర్ణాంకం fd;
	HANDLE h;
	విభజన_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#నిర్వచితమైతే HAVE_GET_FILE_కొలత_EX
	LARGE_INTEGER filesize;
#లేకపోతే
	DWORD filesize;
#అయితే_ముగింపు /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ఒకవేళ (fd < 0)
		తిరిగి errno;
	h = (HANDLE)_get_osfhandle(fd);
	ఒకవేళ (DeviceIoControl(h, IOCTL_DISK_GET_విభజన_INFO,
			    &pi, పరిమాణం(విభజన_INFORMATION),
			    &pi, పరిమాణం(విభజన_INFORMATION),
			    &retbytes, ఖాళీ)) {

		*retblocks = pi.విభజన_నిడివి.QuadPart / blocksize;

	} లేకపోతే ఒకవేళ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, పరిమాణం(DISK_GEOMETRY),
				&gi, పరిమాణం(DISK_GEOMETRY),
				&retbytes, ఖాళీ)) {

		*retblocks = gi.బైట్లు_చొప్పున_Sector *
			     gi.Sectors_చొప్పున_Track *
			     gi.Tracks_చొప్పున_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#నిర్వచితమైతే HAVE_GET_FILE_కొలత_EX
	} లేకపోతే ఒకవేళ (Get_File_కొలత_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#లేకపోతే
	} లేకపోతే {
		filesize = Get_File_కొలత(h, ఖాళీ);
		ఒకవేళ (INVALID_FILE_కొలత != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#అయితే_ముగింపు /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	తిరిగి 0;
}

#లేకపోతే

స్థిర పూర్ణాంకం valid_స్థానభ్రంశం (పూర్ణాంకం fd, ext2_loff_t స్థానభ్రంశం)
{
	అక్షరం ch;

	ఒకవేళ (ext2fs_llseek (fd, స్థానభ్రంశం, 0) < 0)
		తిరిగి 0;
	ఒకవేళ (పఠ (fd, &ch, 1) < 1)
		తిరిగి 0;
	తిరిగి 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
				  blk64_t *retblocks)
{
	పూర్ణాంకం	fd, rc = 0;
	గుర్తులేని దీర్ఘ దీర్ఘ size64;
	ext2_loff_t ఎగువ, దిగువ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ఒకవేళ (fd < 0)
		తిరిగి errno;

#ఒకవేళ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	గుర్తులేని పూర్ణాంకం కొలత;

	ఒకవేళ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &కొలత) >= 0) {
		*retblocks = size64 * కొలత / blocksize;
		వెళ్ళు బహిర్గతం;
	}
#అయితే_ముగింపు

#నిర్వచితమైతే BLKGETSIZE64
	{
		పూర్ణాంకం valid_blkgetsize64 = 1;
#నిర్వచితమైతే __linux__
		నిర్మాణం utsname ut;

		ఒకవేళ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#అయితే_ముగింపు
		ఒకవేళ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			వెళ్ళు బహిర్గతం;
		}
	}
#అయితే_ముగింపు /* BLKGETSIZE64 */

#నిర్వచితమైతే BLKGETSIZE
	{
		గుర్తులేని దీర్ఘ	కొలత;

		ఒకవేళ (ioctl(fd, BLKGETSIZE, &కొలత) >= 0) {
			*retblocks = కొలత / (blocksize / 512);
			వెళ్ళు బహిర్గతం;
		}
	}
#అయితే_ముగింపు

#నిర్వచితమైతే FDGETPRM
	{
		నిర్మాణం floppy_struct this_floppy;

		ఒకవేళ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.కొలత / (blocksize / 512);
			వెళ్ళు బహిర్గతం;
		}
	}
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_SYS_DISKLABEL_H
	{
		పూర్ణాంకం part;
		నిర్మాణం disklabel lab;
		నిర్మాణం విభజన_2 *pp;
		అక్షరం ch;

#ఒకవేళ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ఒకవేళ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				వెళ్ళు బహిర్గతం;
			}
		}
#లేదావేళ defined(DIOCGDINFO)
		/* old disklabel interface */
		part = అక్షరశ్రేణి_పొడవు(file) - 1;
		ఒకవేళ (part >= 0) {
			ch = file[part];
			ఒకవేళ (అంకెనా(ch))
				part = 0;
			లేకపోతే ఒకవేళ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			లేకపోతే
				part = -1;
		}
		ఒకవేళ (part >= 0 && (ioctl(fd, DIOCGDINFO, (అక్షరం *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ఒకవేళ (pp->p_కొలత) {
				*retblocks = pp->p_కొలత / (blocksize / 512);
				వెళ్ళు బహిర్గతం;
			}
		}
#అయితే_ముగింపు /* defined(DIOCG*) */
	}
#అయితే_ముగింపు /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ఒకవేళ (ext2fs_fstat(fd, &st) == 0)
			ఒకవేళ (S_ISREG(st.st_బహుళకం)) {
				*retblocks = st.st_కొలత / blocksize;
				వెళ్ళు బహిర్గతం;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	దిగువ = 0;
	కొరకు (ఎగువ = 1024; valid_స్థానభ్రంశం(fd, ఎగువ); ఎగువ *= 2)
		దిగువ = ఎగువ;
	అయితే (దిగువ < ఎగువ - 1) {
		స్థిరరాశి ext2_loff_t mid = (దిగువ + ఎగువ) / 2;

		ఒకవేళ (valid_స్థానభ్రంశం (fd, mid))
			దిగువ = mid;
		లేకపోతే
			ఎగువ = mid;
	}
	valid_స్థానభ్రంశం(fd, 0);
	size64 = దిగువ + 1;
	*retblocks = size64 / blocksize;
బహిర్గతం:
	close(fd);
	తిరిగి rc;
}

#అయితే_ముగింపు /* WIN32 */

errcode_t ext2fs_get_device_కొలత(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	ఖండాలు;

	retval = ext2fs_get_device_size2(file, blocksize, &ఖండాలు);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (ఖండాలు >= (1ULL << 32))
		తిరిగి EFBIG;
	*retblocks = (blk_t) ఖండాలు;
	తిరిగి 0;
}

#నిర్వచితమైతే DEBUG
పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	blk64_t	ఖండాలు;
	పూర్ణాంకం	retval;

	ఒకవేళ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		నిష్క్రమించు(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &ఖండాలు);
	ఒకవేళ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		నిష్క్రమించు(1);
	}
	రాయి("Device %s has %llu 1k blocks.\n", argv[1],
	       (గుర్తులేని దీర్ఘ దీర్ఘ) locks);
	నిష్క్రమించు(0);
}
#అయితే_ముగింపు
