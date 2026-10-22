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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#ifdef HAVE_SYS_IOCTL_H
#അന്തര്ഭാവ <sys/ioctl.h>
#endif
#ifdef HAVE_LINUX_FD_H
#അന്തര്ഭാവ <linux/fd.h>
#endif
#ifdef HAVE_SYS_DISKLABEL_H
#അന്തര്ഭാവ <sys/disklabel.h>
#endif
#ifdef HAVE_SYS_DISK_H
#അന്തര്ഭാവ <sys/disk.h>
#endif
#ifdef __linux__
#അന്തര്ഭാവ <sys/utsname.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#അന്തര്ഭാവ <ctype.h>

#എങ്കിൽ defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#define BLKGETSIZE _IO(0x12,96)	/* return device size */
#endif

#എങ്കിൽ defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#define BLKGETSIZE64 _IOR(0x12,114,size_t)	/* return device size in bytes (u64 *arg) */
#endif

#ifdef APPLE_DARWIN
#define BLKGETSIZE DKIOCGETBLOCKCOUNT32
#endif /* APPLE_DARWIN */

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#എങ്കിൽ defined(__CYGWIN__) || defined (WIN32)
#അന്തര്ഭാവ "windows.h"
#അന്തര്ഭാവ "winioctl.h"

#എങ്കിൽ (_WIN32_WINNT >= 0x0500)
#define HAVE_GET_FILE_SIZE_EX 1
#endif

errcode_t ext2fs_get_device_size2(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
				  blk64_t *retblocks)
{
	പൂർണ്ണസംഖ്യ fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#ifdef HAVE_GET_FILE_SIZE_EX
	LARGE_INTEGER filesize;
#അല്ലെങ്കിൽ
	DWORD filesize;
#endif /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ errno;
	h = (HANDLE)_get_osfhandle(fd);
	എങ്കിൽ (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, വലുപ്പം(PARTITION_INFORMATION),
			    &pi, വലുപ്പം(PARTITION_INFORMATION),
			    &retbytes, ശൂന്യ)) {

		*retblocks = pi.PartitionLength.QuadPart / blocksize;

	} അല്ലെങ്കിൽ എങ്കിൽ (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, വലുപ്പം(DISK_GEOMETRY),
				&gi, വലുപ്പം(DISK_GEOMETRY),
				&retbytes, ശൂന്യ)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#ifdef HAVE_GET_FILE_SIZE_EX
	} അല്ലെങ്കിൽ എങ്കിൽ (GetFileSizeEx(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#അല്ലെങ്കിൽ
	} അല്ലെങ്കിൽ {
		filesize = GetFileSize(h, ശൂന്യ);
		എങ്കിൽ (INVALID_FILE_SIZE != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#endif /* HAVE_GET_FILE_SIZE_EX */

	close(fd);
	തിരികെ 0;
}

#അല്ലെങ്കിൽ

സ്ഥിതം പൂർണ്ണസംഖ്യ valid_offset (പൂർണ്ണസംഖ്യ fd, ext2_loff_t offset)
{
	അക്ഷരം ch;

	എങ്കിൽ (ext2fs_llseek (fd, offset, 0) < 0)
		തിരികെ 0;
	എങ്കിൽ (പഠ (fd, &ch, 1) < 1)
		തിരികെ 0;
	തിരികെ 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
				  blk64_t *retblocks)
{
	പൂർണ്ണസംഖ്യ	fd, rc = 0;
	unsigned long long size64;
	ext2_loff_t മുകളിലെ, താഴെയുള്ള;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	എങ്കിൽ (fd < 0)
		തിരികെ errno;

#എങ്കിൽ defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	unsigned പൂർണ്ണസംഖ്യ size;

	എങ്കിൽ (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &size) >= 0) {
		*retblocks = size64 * size / blocksize;
		പോകുക out;
	}
#endif

#ifdef BLKGETSIZE64
	{
		പൂർണ്ണസംഖ്യ valid_blkgetsize64 = 1;
#ifdef __linux__
		ഘടന utsname ut;

		എങ്കിൽ ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#endif
		എങ്കിൽ (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			പോകുക out;
		}
	}
#endif /* BLKGETSIZE64 */

#ifdef BLKGETSIZE
	{
		unsigned long	size;

		എങ്കിൽ (ioctl(fd, BLKGETSIZE, &size) >= 0) {
			*retblocks = size / (blocksize / 512);
			പോകുക out;
		}
	}
#endif

#ifdef FDGETPRM
	{
		ഘടന floppy_struct this_floppy;

		എങ്കിൽ (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.size / (blocksize / 512);
			പോകുക out;
		}
	}
#endif

#ifdef HAVE_SYS_DISKLABEL_H
	{
		പൂർണ്ണസംഖ്യ part;
		ഘടന disklabel lab;
		ഘടന partition *pp;
		അക്ഷരം ch;

#എങ്കിൽ defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			എങ്കിൽ (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				പോകുക out;
			}
		}
#അല്ലെങ്കിലെങ്കിൽ defined(DIOCGDINFO)
		/* old disklabel interface */
		part = strlen(file) - 1;
		എങ്കിൽ (part >= 0) {
			ch = file[part];
			എങ്കിൽ (isdigit(ch))
				part = 0;
			അല്ലെങ്കിൽ എങ്കിൽ (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			അല്ലെങ്കിൽ
				part = -1;
		}
		എങ്കിൽ (part >= 0 && (ioctl(fd, DIOCGDINFO, (അക്ഷരം *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			എങ്കിൽ (pp->p_size) {
				*retblocks = pp->p_size / (blocksize / 512);
				പോകുക out;
			}
		}
#endif /* defined(DIOCG*) */
	}
#endif /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		എങ്കിൽ (ext2fs_fstat(fd, &st) == 0)
			എങ്കിൽ (S_ISREG(st.st_mode)) {
				*retblocks = st.st_size / blocksize;
				പോകുക out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	താഴെയുള്ള = 0;
	വേണ്ടി (മുകളിലെ = 1024; valid_offset(fd, മുകളിലെ); മുകളിലെ *= 2)
		താഴെയുള്ള = മുകളിലെ;
	ആകുമ്പോൾ (താഴെയുള്ള < മുകളിലെ - 1) {
		സ്ഥിരം ext2_loff_t mid = (താഴെയുള്ള + മുകളിലെ) / 2;

		എങ്കിൽ (valid_offset (fd, mid))
			താഴെയുള്ള = mid;
		അല്ലെങ്കിൽ
			മുകളിലെ = mid;
	}
	valid_offset(fd, 0);
	size64 = താഴെയുള്ള + 1;
	*retblocks = size64 / blocksize;
out:
	close(fd);
	തിരികെ rc;
}

#endif /* WIN32 */

errcode_t ext2fs_get_device_size(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (blocks >= (1ULL << 32))
		തിരികെ EFBIG;
	*retblocks = (blk_t) blocks;
	തിരികെ 0;
}

#ifdef DEBUG
പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	blk64_t	blocks;
	പൂർണ്ണസംഖ്യ	retval;

	എങ്കിൽ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	എങ്കിൽ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		exit(1);
	}
	printf("Device %s has %llu 1k blocks.\n", argv[1],
	       (unsigned long long) locks);
	exit(0);
}
#endif
