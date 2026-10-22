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

#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <fcntl.h>
#که_تعریف_شوی HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#د_که_پای
#که_تعریف_شوی HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_DISKLABEL_H
#ضمن <sys/disklabel.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_DISK_H
#ضمن <sys/disk.h>
#د_که_پای
#که_تعریف_شوی __linux__
#ضمن <sys/utsname.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#ضمن <ctype.h>

#إذا defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#تعریف_کړه BLKGETSIZE _IO(0x12,96)	/* return device size */
#د_که_پای

#إذا defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#تعریف_کړه BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#د_که_پای

#که_تعریف_شوی APPLE_DARWIN
#تعریف_کړه BLKGETSIZE DKIOCGETBLOCKCOUNT32
#د_که_پای /* APPLE_DARWIN */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#إذا defined(__CYGWIN__) || defined (WIN32)
#ضمن "windows.h"
#ضمن "winioctl.h"

#إذا (_WIN32_WINNT >= 0x0500)
#تعریف_کړه HAVE_GET_FILE_اندازه_EX 1
#د_که_پای

errcode_t ext2fs_get_device_size2(ثابت حرف *file, صحيح blocksize,
				  blk64_t *retblocks)
{
	صحيح fd;
	HANDLE h;
	برخه_کول_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#که_تعریف_شوی HAVE_GET_FILE_اندازه_EX
	LARGE_INTEGER filesize;
#وإلا
	DWORD filesize;
#د_که_پای /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;
	h = (HANDLE)_get_osfhandle(fd);
	إذا (DeviceIoControl(h, IOCTL_DISK_GET_برخه_کول_INFO,
			    &pi, حجم(برخه_کول_INFORMATION),
			    &pi, حجم(برخه_کول_INFORMATION),
			    &retbytes, عدم)) {

		*retblocks = pi.برخه_کول_اوږدتیا.QuadPart / blocksize;

	} وإلا إذا (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY),
				&retbytes, عدم)) {

		*retblocks = gi.بایټونه_فی_Sector *
			     gi.Sectors_فی_Track *
			     gi.Tracks_فی_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#که_تعریف_شوی HAVE_GET_FILE_اندازه_EX
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
#د_که_پای /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	أرجع 0;
}

#وإلا

ساكن صحيح valid_دننه_واټن (صحيح fd, ext2_loff_t دننه_واټن)
{
	حرف ch;

	إذا (ext2fs_llseek (fd, دننه_واټن, 0) < 0)
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
	بینښه اوږد اوږد size64;
	ext2_loff_t لوړ, ټیټ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	إذا (fd < 0)
		أرجع errno;

#إذا defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	بینښه صحيح اندازه;

	إذا (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &اندازه) >= 0) {
		*retblocks = size64 * اندازه / blocksize;
		اذهب بهر;
	}
#د_که_پای

#که_تعریف_شوی BLKGETSIZE64
	{
		صحيح valid_blkgetsize64 = 1;
#که_تعریف_شوی __linux__
		بنية utsname ut;

		إذا ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#د_که_پای
		إذا (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			اذهب بهر;
		}
	}
#د_که_پای /* BLKGETSIZE64 */

#که_تعریف_شوی BLKGETSIZE
	{
		بینښه اوږد	اندازه;

		إذا (ioctl(fd, BLKGETSIZE, &اندازه) >= 0) {
			*retblocks = اندازه / (blocksize / 512);
			اذهب بهر;
		}
	}
#د_که_پای

#که_تعریف_شوی FDGETPRM
	{
		بنية floppy_struct this_floppy;

		إذا (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.اندازه / (blocksize / 512);
			اذهب بهر;
		}
	}
#د_که_پای

#که_تعریف_شوی HAVE_SYS_DISKLABEL_H
	{
		صحيح part;
		بنية disklabel lab;
		بنية برخه_کول_2 *pp;
		حرف ch;

#إذا defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			إذا (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				اذهب بهر;
			}
		}
#وإلاإذا defined(DIOCGDINFO)
		/* old disklabel interface */
		part = مزي_اوږدوالی(file) - 1;
		إذا (part >= 0) {
			ch = file[part];
			إذا (که_عدد(ch))
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
				اذهب بهر;
			}
		}
#د_که_پای /* defined(DIOCG*) */
	}
#د_که_پای /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		إذا (ext2fs_fstat(fd, &st) == 0)
			إذا (S_ISREG(st.st_ډېرپېښ)) {
				*retblocks = st.st_اندازه / blocksize;
				اذهب بهر;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	ټیټ = 0;
	لكل (لوړ = 1024; valid_دننه_واټن(fd, لوړ); لوړ *= 2)
		ټیټ = لوړ;
	طالما (ټیټ < لوړ - 1) {
		ثابت ext2_loff_t mid = (ټیټ + لوړ) / 2;

		إذا (valid_دننه_واټن (fd, mid))
			ټیټ = mid;
		وإلا
			لوړ = mid;
	}
	valid_دننه_واټن(fd, 0);
	size64 = ټیټ + 1;
	*retblocks = size64 / blocksize;
بهر:
	close(fd);
	أرجع rc;
}

#د_که_پای /* WIN32 */

errcode_t ext2fs_get_device_اندازه(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	بلاکونه;

	retval = ext2fs_get_device_size2(file, blocksize, &بلاکونه);
	إذا (retval)
		أرجع retval;
	إذا (بلاکونه >= (1ULL << 32))
		أرجع EFBIG;
	*retblocks = (blk_t) بلاکونه;
	أرجع 0;
}

#که_تعریف_شوی DEBUG
صحيح اصلي(صحيح argc, حرف **argv)
{
	blk64_t	بلاکونه;
	صحيح	retval;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		وتل(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاکونه);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		وتل(1);
	}
	چاپ_کړه("Device %s has %llu 1k blocks.\n", argv[1],
	       (بینښه اوږد اوږد) locks);
	وتل(0);
}
#د_که_پای
