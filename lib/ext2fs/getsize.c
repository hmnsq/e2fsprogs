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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_IOCTL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_FD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/fd.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_DISKLABEL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/disklabel.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_DISK_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/disk.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/utsname.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>

#ਜੇ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKGETSIZE _IO(0x12,96)	/* return device size */
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ APPLE_DARWIN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLKGETSIZE DKIOCGETBLOCKCOUNT32
#ਜੇ_ਅੰਤ /* APPLE_DARWIN */

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਜੇ defined(__CYGWIN__) || defined (WIN32)
#ਅਨ੍ਤਰ੍ਭਾਵ "windows.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "winioctl.h"

#ਜੇ (_WIN32_WINNT >= 0x0500)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_GET_FILE_ਮਾਪ_EX 1
#ਜੇ_ਅੰਤ

errcode_t ext2fs_get_device_size2(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
				  blk64_t *retblocks)
{
	ਪੂਰਨ fd;
	HANDLE h;
	ਹਿੱਸੇ_ਕਰੋ_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GET_FILE_ਮਾਪ_EX
	LARGE_INTEGER filesize;
#ਨਹੀਂਤਾਂ
	DWORD filesize;
#ਜੇ_ਅੰਤ /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ errno;
	h = (HANDLE)_get_osfhandle(fd);
	ਜੇ (DeviceIoControl(h, IOCTL_DISK_GET_ਹਿੱਸੇ_ਕਰੋ_INFO,
			    &pi, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ_INFORMATION),
			    &pi, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ_INFORMATION),
			    &retbytes, ਨਿਰਾ)) {

		*retblocks = pi.ਹਿੱਸੇ_ਕਰੋ_ਲੰਮਾਈ.QuadPart / blocksize;

	} ਨਹੀਂਤਾਂ ਜੇ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ਆਕਾਰ(DISK_GEOMETRY),
				&gi, ਆਕਾਰ(DISK_GEOMETRY),
				&retbytes, ਨਿਰਾ)) {

		*retblocks = gi.ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_Sector *
			     gi.Sectors_ਪ੍ਰਤੀ_Track *
			     gi.Tracks_ਪ੍ਰਤੀ_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GET_FILE_ਮਾਪ_EX
	} ਨਹੀਂਤਾਂ ਜੇ (Get_File_ਮਾਪ_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ਨਹੀਂਤਾਂ
	} ਨਹੀਂਤਾਂ {
		filesize = Get_File_ਮਾਪ(h, ਨਿਰਾ);
		ਜੇ (INVALID_FILE_ਮਾਪ != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#ਜੇ_ਅੰਤ /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਪੂਰਨ valid_ਵਿੱਥ (ਪੂਰਨ fd, ext2_loff_t ਵਿੱਥ)
{
	ਅੱਖਰ ch;

	ਜੇ (ext2fs_llseek (fd, ਵਿੱਥ, 0) < 0)
		ਵਾਪਸ 0;
	ਜੇ (ਪਠ (fd, &ch, 1) < 1)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
				  blk64_t *retblocks)
{
	ਪੂਰਨ	fd, rc = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ size64;
	ext2_loff_t ਉੱਚਾ, ਨੀਵਾਂ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ਜੇ (fd < 0)
		ਵਾਪਸ errno;

#ਜੇ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ;

	ਜੇ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &ਮਾਪ) >= 0) {
		*retblocks = size64 * ਮਾਪ / blocksize;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKGETSIZE64
	{
		ਪੂਰਨ valid_blkgetsize64 = 1;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
		ਬਣਤਰ utsname ut;

		ਜੇ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#ਜੇ_ਅੰਤ
		ਜੇ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}
#ਜੇ_ਅੰਤ /* BLKGETSIZE64 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BLKGETSIZE
	{
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	ਮਾਪ;

		ਜੇ (ioctl(fd, BLKGETSIZE, &ਮਾਪ) >= 0) {
			*retblocks = ਮਾਪ / (blocksize / 512);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ FDGETPRM
	{
		ਬਣਤਰ floppy_struct this_floppy;

		ਜੇ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.ਮਾਪ / (blocksize / 512);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_DISKLABEL_H
	{
		ਪੂਰਨ part;
		ਬਣਤਰ disklabel lab;
		ਬਣਤਰ ਹਿੱਸੇ_ਕਰੋ_2 *pp;
		ਅੱਖਰ ch;

#ਜੇ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ਜੇ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				ਜਾਓ ਬਾਹਰਲਾ;
			}
		}
#ਜਾਂਜੇ defined(DIOCGDINFO)
		/* old disklabel interface */
		part = ਲੜੀ_ਲੰਬਾਈ(file) - 1;
		ਜੇ (part >= 0) {
			ch = file[part];
			ਜੇ (ਅੰਕ_ਹੈ(ch))
				part = 0;
			ਨਹੀਂਤਾਂ ਜੇ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ਨਹੀਂਤਾਂ
				part = -1;
		}
		ਜੇ (part >= 0 && (ioctl(fd, DIOCGDINFO, (ਅੱਖਰ *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ਜੇ (pp->p_ਮਾਪ) {
				*retblocks = pp->p_ਮਾਪ / (blocksize / 512);
				ਜਾਓ ਬਾਹਰਲਾ;
			}
		}
#ਜੇ_ਅੰਤ /* defined(DIOCG*) */
	}
#ਜੇ_ਅੰਤ /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ਜੇ (ext2fs_fstat(fd, &st) == 0)
			ਜੇ (S_ISREG(st.st_ਬਹੁਲਕ)) {
				*retblocks = st.st_ਮਾਪ / blocksize;
				ਜਾਓ ਬਾਹਰਲਾ;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	ਨੀਵਾਂ = 0;
	ਲਈ (ਉੱਚਾ = 1024; valid_ਵਿੱਥ(fd, ਉੱਚਾ); ਉੱਚਾ *= 2)
		ਨੀਵਾਂ = ਉੱਚਾ;
	ਜਦੋਂ (ਨੀਵਾਂ < ਉੱਚਾ - 1) {
		ਅਚਲ ext2_loff_t mid = (ਨੀਵਾਂ + ਉੱਚਾ) / 2;

		ਜੇ (valid_ਵਿੱਥ (fd, mid))
			ਨੀਵਾਂ = mid;
		ਨਹੀਂਤਾਂ
			ਉੱਚਾ = mid;
	}
	valid_ਵਿੱਥ(fd, 0);
	size64 = ਨੀਵਾਂ + 1;
	*retblocks = size64 / blocksize;
ਬਾਹਰਲਾ:
	close(fd);
	ਵਾਪਸ rc;
}

#ਜੇ_ਅੰਤ /* WIN32 */

errcode_t ext2fs_get_device_ਮਾਪ(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	ਖੰਡਾਂ;

	retval = ext2fs_get_device_size2(file, blocksize, &ਖੰਡਾਂ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (ਖੰਡਾਂ >= (1ULL << 32))
		ਵਾਪਸ EFBIG;
	*retblocks = (blk_t) ਖੰਡਾਂ;
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	blk64_t	ਖੰਡਾਂ;
	ਪੂਰਨ	retval;

	ਜੇ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &ਖੰਡਾਂ);
	ਜੇ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("Device %s has %llu 1k blocks.\n", argv[1],
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) locks);
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}
#ਜੇ_ਅੰਤ
