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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#اگر_تعریف_شدہ HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_LINUX_FD_H
#شامل <linux/fd.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_DISKLABEL_H
#شامل <sys/disklabel.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_DISK_H
#شامل <sys/disk.h>
#اگر_ختم
#اگر_تعریف_شدہ __linux__
#شامل <sys/utsname.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#شامل <ctype.h>

#اگر defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#تعریف_کرو BLKGETSIZE _IO(0x12,96)	/* return device size */
#اگر_ختم

#اگر defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#تعریف_کرو BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#اگر_ختم

#اگر_تعریف_شدہ APPLE_DARWIN
#تعریف_کرو BLKGETSIZE DKIOCGETBLOCKCOUNT32
#اگر_ختم /* APPLE_DARWIN */

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#اگر defined(__CYGWIN__) || defined (WIN32)
#شامل "windows.h"
#شامل "winioctl.h"

#اگر (_WIN32_WINNT >= 0x0500)
#تعریف_کرو HAVE_GET_FILE_سائز_EX 1
#اگر_ختم

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحیح blocksize,
				  blk64_t *retblocks)
{
	صحیح fd;
	HANDLE h;
	تقسیم_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#اگر_تعریف_شدہ HAVE_GET_FILE_سائز_EX
	LARGE_INTEGER filesize;
#ورنہ
	DWORD filesize;
#اگر_ختم /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		واپس errno;
	h = (HANDLE)_get_osfhandle(fd);
	اگر (DeviceIoControl(h, IOCTL_DISK_GET_تقسیم_INFO,
			    &pi, حجم(تقسیم_INFORMATION),
			    &pi, حجم(تقسیم_INFORMATION),
			    &retbytes, تہی)) {

		*retblocks = pi.تقسیم_لمبائی.QuadPart / blocksize;

	} ورنہ اگر (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, تہی)) {

		*retblocks = gi.بائٹیں_فی_Sector *
			     gi.Sectors_فی_Track *
			     gi.Tracks_فی_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#اگر_تعریف_شدہ HAVE_GET_FILE_سائز_EX
	} ورنہ اگر (Get_File_سائز_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ورنہ
	} ورنہ {
		filesize = Get_File_سائز(h, تہی);
		اگر (INVALID_FILE_سائز != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#اگر_ختم /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	واپس 0;
}

#ورنہ

ساکن صحیح valid_انحراف (صحیح fd, ext2_loff_t انحراف)
{
	حرف ch;

	اگر (ext2fs_llseek (fd, انحراف, 0) < 0)
		واپس 0;
	اگر (اقرأ (fd, &ch, 1) < 1)
		واپس 0;
	واپس 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحیح blocksize,
				  blk64_t *retblocks)
{
	صحیح	fd, rc = 0;
	غیرعلامتی طویل طویل size64;
	ext2_loff_t بلند, کم;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	اگر (fd < 0)
		واپس errno;

#اگر defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	غیرعلامتی صحیح سائز;

	اگر (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &سائز) >= 0) {
		*retblocks = size64 * سائز / blocksize;
		جاؤ برآمد;
	}
#اگر_ختم

#اگر_تعریف_شدہ BLKGETSIZE64
	{
		صحیح valid_blkgetsize64 = 1;
#اگر_تعریف_شدہ __linux__
		ساخت utsname ut;

		اگر ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#اگر_ختم
		اگر (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			جاؤ برآمد;
		}
	}
#اگر_ختم /* BLKGETSIZE64 */

#اگر_تعریف_شدہ BLKGETSIZE
	{
		غیرعلامتی طویل	سائز;

		اگر (ioctl(fd, BLKGETSIZE, &سائز) >= 0) {
			*retblocks = سائز / (blocksize / 512);
			جاؤ برآمد;
		}
	}
#اگر_ختم

#اگر_تعریف_شدہ FDGETPRM
	{
		ساخت floppy_struct this_floppy;

		اگر (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.سائز / (blocksize / 512);
			جاؤ برآمد;
		}
	}
#اگر_ختم

#اگر_تعریف_شدہ HAVE_SYS_DISKLABEL_H
	{
		صحیح part;
		ساخت disklabel lab;
		ساخت تقسیم_2 *pp;
		حرف ch;

#اگر defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			اگر (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				جاؤ برآمد;
			}
		}
#وگرنہ_اگر defined(DIOCGDINFO)
		/* old disklabel interface */
		part = لڑی_لمبائی(file) - 1;
		اگر (part >= 0) {
			ch = file[part];
			اگر (ہندسہ_ہے(ch))
				part = 0;
			ورنہ اگر (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ورنہ
				part = -1;
		}
		اگر (part >= 0 && (ioctl(fd, DIOCGDINFO, (حرف *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			اگر (pp->p_سائز) {
				*retblocks = pp->p_سائز / (blocksize / 512);
				جاؤ برآمد;
			}
		}
#اگر_ختم /* defined(DIOCG*) */
	}
#اگر_ختم /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		اگر (ext2fs_fstat(fd, &st) == 0)
			اگر (S_ISREG(st.st_منوال)) {
				*retblocks = st.st_سائز / blocksize;
				جاؤ برآمد;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	کم = 0;
	برائے (بلند = 1024; valid_انحراف(fd, بلند); بلند *= 2)
		کم = بلند;
	جب (کم < بلند - 1) {
		ثابت ext2_loff_t mid = (کم + بلند) / 2;

		اگر (valid_انحراف (fd, mid))
			کم = mid;
		ورنہ
			بلند = mid;
	}
	valid_انحراف(fd, 0);
	size64 = کم + 1;
	*retblocks = size64 / blocksize;
برآمد:
	close(fd);
	واپس rc;
}

#اگر_ختم /* WIN32 */

errcode_t ext2fs_get_device_سائز(ثابت حرف *file, صحیح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	بلاکیں;

	retval = ext2fs_get_device_size2(file, blocksize, &بلاکیں);
	اگر (retval)
		واپس retval;
	اگر (بلاکیں >= (1ULL << 32))
		واپس EFBIG;
	*retblocks = (blk_t) بلاکیں;
	واپس 0;
}

#اگر_تعریف_شدہ DEBUG
صحیح مین(صحیح argc, حرف **argv)
{
	blk64_t	بلاکیں;
	صحیح	retval;

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		باہر_نکلو(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاکیں);
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		باہر_نکلو(1);
	}
	لکھو("Device %s has %llu 1k blocks.\n", argv[1],
	       (غیرعلامتی طویل طویل) locks);
	باہر_نکلو(0);
}
#اگر_ختم
