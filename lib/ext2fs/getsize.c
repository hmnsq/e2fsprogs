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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#اگر_تعریف_شده HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_DISKLABEL_H
#ضمن <sys/disklabel.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#پایان_اگر
#اگر_تعریف_شده __linux__
#ضمن <sys/utsname.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#ضمن <ctype.h>

#إذا defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#تعریف_کن BLKGETSIZE _IO(0x12,96)	/* return device size */
#پایان_اگر

#إذا defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#تعریف_کن BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#پایان_اگر

#اگر_تعریف_شده APPLE_DARWIN
#تعریف_کن BLKGETSIZE DKIOCGETBLOCKCOUNT32
#پایان_اگر /* APPLE_DARWIN */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#إذا defined(__CYGWIN__) || defined (WIN32)
#ضمن "windows.h"
#ضمن "winioctl.h"

#إذا (_WIN32_WINNT >= 0x0500)
#تعریف_کن HAVE_GET_FILE_اندازه_EX 1
#پایان_اگر

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح fd;
	HANDLE h;
	افراز_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#اگر_تعریف_شده HAVE_GET_FILE_اندازه_EX
	LARGE_INTEGER filesize;
#وإلا
	DWORD filesize;
#پایان_اگر /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;
	h = (HANDLE)_get_osfhandle(fd);
	إذا (DeviceIoControl(h, IOCTL_DISK_GET_افراز_INFO,
			    &pi, حجم(افراز_INFORMATION),
			    &pi, حجم(افراز_INFORMATION),
			    &retbytes, عدم)) {

		*retblocks = pi.افراز_درازی.QuadPart / blocksize;

	} وإلا إذا (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, عدم)) {

		*retblocks = gi.بایتها_در_هر_Sector *
			     gi.Sectors_در_هر_Track *
			     gi.Tracks_در_هر_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#اگر_تعریف_شده HAVE_GET_FILE_اندازه_EX
	} وإلا إذا (Get_File_اندازه_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#وإلا
	} وإلا {
		filesize = Get_File_اندازه(h, عدم);
		إذا (INVALID_FILE_اندازه != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#پایان_اگر /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	أرجع 0;
}

#وإلا

ساكن صحيح valid_جابه_جایی (صحيح fd, ext2_loff_t جابه_جایی)
{
	حرف ch;

	إذا (ext2fs_llseek (fd, جابه_جایی, 0) < 0)
		أرجع 0;
	إذا (اقرأ (fd, &ch, 1) < 1)
		أرجع 0;
	أرجع 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح	fd, rc = 0;
	بیعلامت دراز دراز size64;
	ext2_loff_t بالا, کم;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#إذا defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	بیعلامت صحيح اندازه;

	إذا (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &اندازه) >= 0) {
		*retblocks = size64 * اندازه / blocksize;
		اذهب بیرون;
	}
#پایان_اگر

#اگر_تعریف_شده BLKGETSIZE64
	{
		صحيح valid_blkgetsize64 = 1;
#اگر_تعریف_شده __linux__
		بنية utsname ut;

		إذا ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#پایان_اگر
		إذا (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			اذهب بیرون;
		}
	}
#پایان_اگر /* BLKGETSIZE64 */

#اگر_تعریف_شده BLKGETSIZE
	{
		بیعلامت دراز	اندازه;

		إذا (ioctl(fd, BLKGETSIZE, &اندازه) >= 0) {
			*retblocks = اندازه / (blocksize / 512);
			اذهب بیرون;
		}
	}
#پایان_اگر

#اگر_تعریف_شده FDGETPRM
	{
		بنية floppy_struct this_floppy;

		إذا (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.اندازه / (blocksize / 512);
			اذهب بیرون;
		}
	}
#پایان_اگر

#اگر_تعریف_شده HAVE_SYS_DISKLABEL_H
	{
		صحيح part;
		بنية disklabel lab;
		بنية افراز_2 *pp;
		حرف ch;

#إذا defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			إذا (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				اذهب بیرون;
			}
		}
#وإلاإذا defined(DIOCGDINFO)
		/* old disklabel interface */
		part = طول_رشته(file) - 1;
		إذا (part >= 0) {
			ch = file[part];
			إذا (آیا_رقم(ch))
				part = 0;
			وإلا إذا (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			وإلا
				part = -1;
		}
		إذا (part >= 0 && (ioctl(fd, DIOCGDINFO, (حرف *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			إذا (pp->p_اندازه) {
				*retblocks = pp->p_اندازه / (blocksize / 512);
				اذهب بیرون;
			}
		}
#پایان_اگر /* defined(DIOCG*) */
	}
#پایان_اگر /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		إذا (ext2fs_fstat(fd, &st) == 0)
			إذا (S_ISREG(st.st_نما)) {
				*retblocks = st.st_اندازه / blocksize;
				اذهب بیرون;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	کم = 0;
	لكل (بالا = 1024; valid_جابه_جایی(fd, بالا); بالا *= 2)
		کم = بالا;
	طالما (کم < بالا - 1) {
		ثابت ext2_loff_t mid = (کم + بالا) / 2;

		إذا (valid_جابه_جایی (fd, mid))
			کم = mid;
		وإلا
			بالا = mid;
	}
	valid_جابه_جایی(fd, 0);
	size64 = کم + 1;
	*retblocks = size64 / blocksize;
بیرون:
	close(fd);
	أرجع rc;
}

#پایان_اگر /* WIN32 */

errcode_t ext2fs_get_device_اندازه(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	بلوکها;

	retval = ext2fs_get_device_size2(file, blocksize, &بلوکها);
	إذا (retval)
		أرجع retval;
	إذا (بلوکها >= (1ULL << 32))
		أرجع EFBIG;
	*retblocks = (blk_t) بلوکها;
	أرجع 0;
}

#اگر_تعریف_شده DEBUG
صحيح اصلی(صحيح argc, حرف **argv)
{
	blk64_t	بلوکها;
	صحيح	retval;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		خروج(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &بلوکها);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		خروج(1);
	}
	چاپ_کن("Device %s has %llu 1k blocks.\n", argv[1],
	       (بیعلامت دراز دراز) locks);
	خروج(0);
}
#پایان_اگر
