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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_DISKLABEL_H
#ಅನ್ತರ್ಭಾವ <sys/disklabel.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_DISK_H
#ಅನ್ತರ್ಭಾವ <sys/disk.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
#ಅನ್ತರ್ಭಾವ <sys/utsname.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <ctype.h>

#ಆದರೆ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKGETSIZE _IO(0x12,96)	/* return device size */
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#ವ್ಯಾಖ್ಯಾನಿಸು BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ APPLE_DARWIN
#ವ್ಯಾಖ್ಯಾನಿಸು BLKGETSIZE DKIOCGETBLOCKCOUNT32
#ಆದರೆ_ಅಂತ್ಯ /* APPLE_DARWIN */

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ಆದರೆ defined(__CYGWIN__) || defined (WIN32)
#ಅನ್ತರ್ಭಾವ "windows.h"
#ಅನ್ತರ್ಭಾವ "winioctl.h"

#ಆದರೆ (_WIN32_WINNT >= 0x0500)
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_GET_FILE_ಗಾತ್ರ_EX 1
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_get_device_size2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
				  blk64_t *retblocks)
{
	ಪೂರ್ಣಾಂಕ fd;
	HANDLE h;
	ವಿಭಜನೆ_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GET_FILE_ಗಾತ್ರ_EX
	LARGE_INTEGER filesize;
#ಇಲ್ಲದಿದ್ದರೆ
	DWORD filesize;
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ errno;
	h = (HANDLE)_get_osfhandle(fd);
	ಆದರೆ (DeviceIoControl(h, IOCTL_DISK_GET_ವಿಭಜನೆ_INFO,
			    &pi, ಗಾತ್ರ(ವಿಭಜನೆ_INFORMATION),
			    &pi, ಗಾತ್ರ(ವಿಭಜನೆ_INFORMATION),
			    &retbytes, ಖಾಲಿ)) {

		*retblocks = pi.ವಿಭಜನೆ_ಉದ್ದಳತೆ.QuadPart / blocksize;

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ಗಾತ್ರ(DISK_GEOMETRY),
				&gi, ಗಾತ್ರ(DISK_GEOMETRY),
				&retbytes, ಖಾಲಿ)) {

		*retblocks = gi.ಬೈಟುಗಳು_ಪ್ರತಿ_Sector *
			     gi.Sectors_ಪ್ರತಿ_Track *
			     gi.Tracks_ಪ್ರತಿ_Cylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GET_FILE_ಗಾತ್ರ_EX
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (Get_File_ಗಾತ್ರ_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#ಇಲ್ಲದಿದ್ದರೆ
	} ಇಲ್ಲದಿದ್ದರೆ {
		filesize = Get_File_ಗಾತ್ರ(h, ಖಾಲಿ);
		ಆದರೆ (INVALID_FILE_ಗಾತ್ರ != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	ಹಿಂತಿರುಗಿ 0;
}

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ valid_ಸ್ಥಾನಾಂತರ (ಪೂರ್ಣಾಂಕ fd, ext2_loff_t ಸ್ಥಾನಾಂತರ)
{
	ಅಕ್ಷರ ch;

	ಆದರೆ (ext2fs_llseek (fd, ಸ್ಥಾನಾಂತರ, 0) < 0)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ಪಠ (fd, &ch, 1) < 1)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
				  blk64_t *retblocks)
{
	ಪೂರ್ಣಾಂಕ	fd, rc = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ size64;
	ext2_loff_t ಮೇಲಿನ, ಕೆಳಗಿನ;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ errno;

#ಆದರೆ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;

	ಆದರೆ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &ಗಾತ್ರ_2) >= 0) {
		*retblocks = size64 * ಗಾತ್ರ_2 / blocksize;
		ಹೋಗು ಹೊರಹೋಗು;
	}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKGETSIZE64
	{
		ಪೂರ್ಣಾಂಕ valid_blkgetsize64 = 1;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
		ರಚನೆ utsname ut;

		ಆದರೆ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ /* BLKGETSIZE64 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BLKGETSIZE
	{
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	ಗಾತ್ರ_2;

		ಆದರೆ (ioctl(fd, BLKGETSIZE, &ಗಾತ್ರ_2) >= 0) {
			*retblocks = ಗಾತ್ರ_2 / (blocksize / 512);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ FDGETPRM
	{
		ರಚನೆ floppy_struct this_floppy;

		ಆದರೆ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.ಗಾತ್ರ_2 / (blocksize / 512);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_DISKLABEL_H
	{
		ಪೂರ್ಣಾಂಕ part;
		ರಚನೆ disklabel lab;
		ರಚನೆ ವಿಭಜನೆ_2 *pp;
		ಅಕ್ಷರ ch;

#ಆದರೆ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			ಆದರೆ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				ಹೋಗು ಹೊರಹೋಗು;
			}
		}
#ಅಥವಾಆದರೆ defined(DIOCGDINFO)
		/* old disklabel interface */
		part = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(file) - 1;
		ಆದರೆ (part >= 0) {
			ch = file[part];
			ಆದರೆ (ಅಂಕಿಯೇ(ch))
				part = 0;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			ಇಲ್ಲದಿದ್ದರೆ
				part = -1;
		}
		ಆದರೆ (part >= 0 && (ioctl(fd, DIOCGDINFO, (ಅಕ್ಷರ *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			ಆದರೆ (pp->p_ಗಾತ್ರ) {
				*retblocks = pp->p_ಗಾತ್ರ / (blocksize / 512);
				ಹೋಗು ಹೊರಹೋಗು;
			}
		}
#ಆದರೆ_ಅಂತ್ಯ /* defined(DIOCG*) */
	}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		ಆದರೆ (ext2fs_fstat(fd, &st) == 0)
			ಆದರೆ (S_ISREG(st.st_ಬಹುಲಕ)) {
				*retblocks = st.st_ಗಾತ್ರ / blocksize;
				ಹೋಗು ಹೊರಹೋಗು;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	ಕೆಳಗಿನ = 0;
	ಗಾಗಿ (ಮೇಲಿನ = 1024; valid_ಸ್ಥಾನಾಂತರ(fd, ಮೇಲಿನ); ಮೇಲಿನ *= 2)
		ಕೆಳಗಿನ = ಮೇಲಿನ;
	ಆಗ (ಕೆಳಗಿನ < ಮೇಲಿನ - 1) {
		ಸ್ಥಿರಾಂಕ ext2_loff_t mid = (ಕೆಳಗಿನ + ಮೇಲಿನ) / 2;

		ಆದರೆ (valid_ಸ್ಥಾನಾಂತರ (fd, mid))
			ಕೆಳಗಿನ = mid;
		ಇಲ್ಲದಿದ್ದರೆ
			ಮೇಲಿನ = mid;
	}
	valid_ಸ್ಥಾನಾಂತರ(fd, 0);
	size64 = ಕೆಳಗಿನ + 1;
	*retblocks = size64 / blocksize;
ಹೊರಹೋಗು:
	close(fd);
	ಹಿಂತಿರುಗಿ rc;
}

#ಆದರೆ_ಅಂತ್ಯ /* WIN32 */

errcode_t ext2fs_get_device_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	ಖಂಡಗಳು;

	retval = ext2fs_get_device_size2(file, blocksize, &ಖಂಡಗಳು);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ಖಂಡಗಳು >= (1ULL << 32))
		ಹಿಂತಿರುಗಿ EFBIG;
	*retblocks = (blk_t) ಖಂಡಗಳು;
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	blk64_t	ಖಂಡಗಳು;
	ಪೂರ್ಣಾಂಕ	retval;

	ಆದರೆ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &ಖಂಡಗಳು);
	ಆದರೆ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("Device %s has %llu 1k blocks.\n", argv[1],
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) locks);
	ನಿರ್ಗಮಿಸು(0);
}
#ಆದರೆ_ಅಂತ್ಯ
