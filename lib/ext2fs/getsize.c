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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#جے_تعریف_شدہ HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جے_انت
#جے_تعریف_شدہ HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_DISKLABEL_H
#ضمن <sys/disklabel.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#جے_انت
#جے_تعریف_شدہ __linux__
#ضمن <sys/utsname.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#ضمن <ctype.h>

#إذا defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#تعریف_کرو BLKGETSIZE _IO(0x12,96)	/* return device size */
#جے_انت

#إذا defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#تعریف_کرو BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#جے_انت

#جے_تعریف_شدہ APPLE_DARWIN
#تعریف_کرو BLKGETSIZE DKIOCGETBLOCKCOUNT32
#جے_انت /* APPLE_DARWIN */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#إذا defined(__CYGWIN__) || defined (WIN32)
#ضمن "windows.h"
#ضمن "winioctl.h"

#إذا (_WIN32_WINNT >= 0x0500)
#تعریف_کرو HAVE_GET_FILE_ناپ_EX 1
#جے_انت

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح fd;
	HANDLE h;
	حصے_کرو_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#جے_تعریف_شدہ HAVE_GET_FILE_ناپ_EX
	LARGE_INTEGER filesize;
#وإلا
	DWORD filesize;
#جے_انت /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;
	h = (HANDLE)_get_osfhandle(fd);
	إذا (DeviceIoControl(h, IOCTL_DISK_GET_حصے_کرو_INFO,
			    &pi, حجم(حصے_کرو_INFORMATION),
			    &pi, حجم(حصے_کرو_INFORMATION),
			    &retbytes, عدم)) {

		*retblocks = pi.حصے_کرو_لمائی.QuadPart / blocksize;

	} وإلا إذا (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, عدم)) {

		*retblocks = gi.بائٹاں_فی_Sector *
			     gi.Sectors_فی_Track *
			     gi.Tracks_فی_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#جے_تعریف_شدہ HAVE_GET_FILE_ناپ_EX
	} وإلا إذا (Get_File_ناپ_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#وإلا
	} وإلا {
		filesize = Get_File_ناپ(h, عدم);
		إذا (INVALID_FILE_ناپ != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#جے_انت /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	أرجع 0;
}

#وإلا

ساكن صحيح valid_وتھ (صحيح fd, ext2_loff_t وتھ)
{
	حرف ch;

	إذا (ext2fs_llseek (fd, وتھ, 0) < 0)
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
	بنانشانی لما لما size64;
	ext2_loff_t بلند, کم;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#إذا defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	بنانشانی صحيح ناپ;

	إذا (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &ناپ) >= 0) {
		*retblocks = size64 * ناپ / blocksize;
		اذهب باہرلا;
	}
#جے_انت

#جے_تعریف_شدہ BLKGETSIZE64
	{
		صحيح valid_blkgetsize64 = 1;
#جے_تعریف_شدہ __linux__
		بنية utsname ut;

		إذا ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#جے_انت
		إذا (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			اذهب باہرلا;
		}
	}
#جے_انت /* BLKGETSIZE64 */

#جے_تعریف_شدہ BLKGETSIZE
	{
		بنانشانی لما	ناپ;

		إذا (ioctl(fd, BLKGETSIZE, &ناپ) >= 0) {
			*retblocks = ناپ / (blocksize / 512);
			اذهب باہرلا;
		}
	}
#جے_انت

#جے_تعریف_شدہ FDGETPRM
	{
		بنية floppy_struct this_floppy;

		إذا (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.ناپ / (blocksize / 512);
			اذهب باہرلا;
		}
	}
#جے_انت

#جے_تعریف_شدہ HAVE_SYS_DISKLABEL_H
	{
		صحيح part;
		بنية disklabel lab;
		بنية حصے_کرو_2 *pp;
		حرف ch;

#إذا defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			إذا (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				اذهب باہرلا;
			}
		}
#وإلاإذا defined(DIOCGDINFO)
		/* old disklabel interface */
		part = لڑی_لمبائی(file) - 1;
		إذا (part >= 0) {
			ch = file[part];
			إذا (ہندسہ_اے(ch))
				part = 0;
			وإلا إذا (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			وإلا
				part = -1;
		}
		إذا (part >= 0 && (ioctl(fd, DIOCGDINFO, (حرف *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			إذا (pp->p_ناپ) {
				*retblocks = pp->p_ناپ / (blocksize / 512);
				اذهب باہرلا;
			}
		}
#جے_انت /* defined(DIOCG*) */
	}
#جے_انت /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		إذا (ext2fs_fstat(fd, &st) == 0)
			إذا (S_ISREG(st.st_منوال)) {
				*retblocks = st.st_ناپ / blocksize;
				اذهب باہرلا;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	کم = 0;
	لكل (بلند = 1024; valid_وتھ(fd, بلند); بلند *= 2)
		کم = بلند;
	طالما (کم < بلند - 1) {
		ثابت ext2_loff_t mid = (کم + بلند) / 2;

		إذا (valid_وتھ (fd, mid))
			کم = mid;
		وإلا
			بلند = mid;
	}
	valid_وتھ(fd, 0);
	size64 = کم + 1;
	*retblocks = size64 / blocksize;
باہرلا:
	close(fd);
	أرجع rc;
}

#جے_انت /* WIN32 */

errcode_t ext2fs_get_device_ناپ(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	بلاکاں;

	retval = ext2fs_get_device_size2(file, blocksize, &بلاکاں);
	إذا (retval)
		أرجع retval;
	إذا (بلاکاں >= (1ULL << 32))
		أرجع EFBIG;
	*retblocks = (blk_t) بلاکاں;
	أرجع 0;
}

#جے_تعریف_شدہ DEBUG
صحيح مکھ(صحيح argc, حرف **argv)
{
	blk64_t	بلاکاں;
	صحيح	retval;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		باہر_نکلو(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاکاں);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		باہر_نکلو(1);
	}
	لکھو("Device %s has %llu 1k blocks.\n", argv[1],
	       (بنانشانی لما لما) locks);
	باہر_نکلو(0);
}
#جے_انت
