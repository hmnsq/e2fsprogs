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

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#جيڪڏهن_وصف_ٿيل HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_DISKLABEL_H
#ضمن <sys/disklabel.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل __linux__
#ضمن <sys/utsname.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#ضمن <ctype.h>

#إذا defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#وصف_ڪريو BLKGETSIZE _IO(0x12,96)	/* return device size */
#جيڪڏهن_پورو

#إذا defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#وصف_ڪريو BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل APPLE_DARWIN
#وصف_ڪريو BLKGETSIZE DKIOCGETBLOCKCOUNT32
#جيڪڏهن_پورو /* APPLE_DARWIN */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#إذا defined(__CYGWIN__) || defined (WIN32)
#ضمن "windows.h"
#ضمن "winioctl.h"

#إذا (_WIN32_WINNT >= 0x0500)
#وصف_ڪريو HAVE_GET_FILE_سائيز_EX 1
#جيڪڏهن_پورو

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح fd;
	HANDLE h;
	حصا_ڪريو_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#جيڪڏهن_وصف_ٿيل HAVE_GET_FILE_سائيز_EX
	LARGE_INTEGER filesize;
#وإلا
	DWORD filesize;
#جيڪڏهن_پورو /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;
	h = (HANDLE)_get_osfhandle(fd);
	إذا (DeviceIoControl(h, IOCTL_DISK_GET_حصا_ڪريو_INFO,
			    &pi, حجم(حصا_ڪريو_INFORMATION),
			    &pi, حجم(حصا_ڪريو_INFORMATION),
			    &retbytes, عدم)) {

		*retblocks = pi.حصا_ڪريو_ڊيگهائي.QuadPart / blocksize;

	} وإلا إذا (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, عدم)) {

		*retblocks = gi.بائيٽون_في_Sector *
			     gi.Sectors_في_Track *
			     gi.Tracks_في_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#جيڪڏهن_وصف_ٿيل HAVE_GET_FILE_سائيز_EX
	} وإلا إذا (Get_File_سائيز_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#وإلا
	} وإلا {
		filesize = Get_File_سائيز(h, عدم);
		إذا (INVALID_FILE_سائيز != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#جيڪڏهن_پورو /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	أرجع 0;
}

#وإلا

ساكن صحيح valid_وٿي (صحيح fd, ext2_loff_t وٿي)
{
	حرف ch;

	إذا (ext2fs_llseek (fd, وٿي, 0) < 0)
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
	بنانشان ڊگهو ڊگهو size64;
	ext2_loff_t مٿاهون, هيٺيون;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#إذا defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	بنانشان صحيح سائيز;

	إذا (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &سائيز) >= 0) {
		*retblocks = size64 * سائيز / blocksize;
		اذهب ٻاهريون;
	}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل BLKGETSIZE64
	{
		صحيح valid_blkgetsize64 = 1;
#جيڪڏهن_وصف_ٿيل __linux__
		بنية utsname ut;

		إذا ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#جيڪڏهن_پورو
		إذا (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			اذهب ٻاهريون;
		}
	}
#جيڪڏهن_پورو /* BLKGETSIZE64 */

#جيڪڏهن_وصف_ٿيل BLKGETSIZE
	{
		بنانشان ڊگهو	سائيز;

		إذا (ioctl(fd, BLKGETSIZE, &سائيز) >= 0) {
			*retblocks = سائيز / (blocksize / 512);
			اذهب ٻاهريون;
		}
	}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل FDGETPRM
	{
		بنية floppy_struct this_floppy;

		إذا (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.سائيز / (blocksize / 512);
			اذهب ٻاهريون;
		}
	}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_SYS_DISKLABEL_H
	{
		صحيح part;
		بنية disklabel lab;
		بنية حصا_ڪريو_2 *pp;
		حرف ch;

#إذا defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			إذا (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				اذهب ٻاهريون;
			}
		}
#وإلاإذا defined(DIOCGDINFO)
		/* old disklabel interface */
		part = لڙي_ڊيگهه(file) - 1;
		إذا (part >= 0) {
			ch = file[part];
			إذا (انگ_آهي(ch))
				part = 0;
			وإلا إذا (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			وإلا
				part = -1;
		}
		إذا (part >= 0 && (ioctl(fd, DIOCGDINFO, (حرف *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			إذا (pp->p_سائيز) {
				*retblocks = pp->p_سائيز / (blocksize / 512);
				اذهب ٻاهريون;
			}
		}
#جيڪڏهن_پورو /* defined(DIOCG*) */
	}
#جيڪڏهن_پورو /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		إذا (ext2fs_fstat(fd, &st) == 0)
			إذا (S_ISREG(st.st_گهڻائي)) {
				*retblocks = st.st_سائيز / blocksize;
				اذهب ٻاهريون;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	هيٺيون = 0;
	لكل (مٿاهون = 1024; valid_وٿي(fd, مٿاهون); مٿاهون *= 2)
		هيٺيون = مٿاهون;
	طالما (هيٺيون < مٿاهون - 1) {
		ثابت ext2_loff_t mid = (هيٺيون + مٿاهون) / 2;

		إذا (valid_وٿي (fd, mid))
			هيٺيون = mid;
		وإلا
			مٿاهون = mid;
	}
	valid_وٿي(fd, 0);
	size64 = هيٺيون + 1;
	*retblocks = size64 / blocksize;
ٻاهريون:
	close(fd);
	أرجع rc;
}

#جيڪڏهن_پورو /* WIN32 */

errcode_t ext2fs_get_device_سائيز(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	بلاڪون;

	retval = ext2fs_get_device_size2(file, blocksize, &بلاڪون);
	إذا (retval)
		أرجع retval;
	إذا (بلاڪون >= (1ULL << 32))
		أرجع EFBIG;
	*retblocks = (blk_t) بلاڪون;
	أرجع 0;
}

#جيڪڏهن_وصف_ٿيل DEBUG
صحيح مکيه(صحيح argc, حرف **argv)
{
	blk64_t	بلاڪون;
	صحيح	retval;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ٻاهر_نڪرو(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاڪون);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		ٻاهر_نڪرو(1);
	}
	لکو("Device %s has %llu 1k blocks.\n", argv[1],
	       (بنانشان ڊگهو ڊگهو) locks);
	ٻاهر_نڪرو(0);
}
#جيڪڏهن_پورو
