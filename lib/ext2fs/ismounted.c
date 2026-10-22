/*
 * ismounted.c --- Check to see if the filesystem was mounted
 *
 * Copyright (C) 1995,1996,1997,1998,1999,2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

/* define BSD_SOURCE to make sure we get the major() macro */
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#endif

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#அந்தர்பாவ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#அந்தர்பாவ <linux/loop.h>
#அந்தர்பாவ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#அந்தர்பாவ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#அந்தர்பாவ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#அந்தர்பாவ <paths.h>
#அந்தர்பாவ <sys/param.h>
#அந்தர்பாவ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/stat.h>
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#அந்தர்பாவ <sys/sysmacros.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
நிலையான முழு check_loop_mounted(மாறிலி எழுத்து *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#எனில் defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	கட்டமைப்பு loop_info64 loopinfo = {0, };
	முழு loop_fd, ret;

	எனில் (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		எனில் (loop_fd < 0)
			திரும்பு -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		எனில் (ret < 0)
			திரும்பு -1;

		எனில் (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			திரும்பு 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	திரும்பு 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
நிலையான errcode_t check_mntent_file(மாறிலி எழுத்து *mtab_file, மாறிலி எழுத்து *file,
				   முழு *mount_flags, எழுத்து *mtpt, முழு mtlen)
{
	கட்டமைப்பு mntent 	*mnt;
	கட்டமைப்பு stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	முழு		fd;

	*mount_flags = 0;

	எனில் ((f = setmntent (mtab_file, "r")) == வெற்று) {
		எனில் (errno == ENOENT) {
			எனில் (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				திரும்பு 0;
			இல்லையெனில்
				திரும்பு EXT2_ET_NO_MTAB_FILE;
		}
		திரும்பு errno;
	}
	எனில் (stat(file, &st_buf) == 0) {
		எனில் (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} இல்லையெனில் {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	போது ((mnt = getmntent (f)) != வெற்று) {
		எனில் (mnt->mnt_fsname[0] != '/')
			தொடர்;
		எனில் (strcmp(file, mnt->mnt_fsname) == 0) {
			எனில் (stat(mnt->mnt_dir, &st_buf) != 0)
				தொடர்;
			எனில் (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				தொடர்;
			}
			நிறுத்து;
		}
		எனில் (stat(mnt->mnt_fsname, &st_buf) == 0) {
			எனில் (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				எனில் (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					எனில் (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						தொடர்;
					எனில் (file_rdev == dir_st_buf.st_dev)
						நிறுத்து;
				}
				எனில் (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					நிறுத்து;
#endif	/* __GNU__ */
			} இல்லையெனில் {
				எனில் (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					நிறுத்து;
			}
		}
	}

	எனில் (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		எனில் (file_rdev && stat("/", &st_buf) == 0) {
			எனில் (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				எனில் (mtpt)
					strncpy(mtpt, "/", mtlen);
				செல் is_வேர்;
			}
		}
#endif	/* __GNU__ */
		செல் errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	எனில் (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	எனில் (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	எனில் (!strcmp(mnt->mnt_dir, "/")) {
is_வேர்:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		எனில் (fd < 0) {
			எனில் (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} இல்லையெனில்
			close(fd);
		(வெறுமை) unlink(TEST_FILE);
	}

	எனில் (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	திரும்பு retval;
}

நிலையான errcode_t check_mntent(மாறிலி எழுத்து *file, முழு *mount_flags,
			      எழுத்து *mtpt, முழு mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	எனில் (retval == 0)
		திரும்பு 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	எனில் (retval == 0)
		திரும்பு 0;
#endif /* __linux__ */
#எனில் defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	திரும்பு retval;
#இல்லையெனில்
	*mount_flags = 0;
	திரும்பு 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#இல்லையெனில்
#எனில் defined(HAVE_GETMNTINFO)

நிலையான errcode_t check_getmntinfo(மாறிலி எழுத்து *file, முழு *mount_flags,
				  எழுத்து *mtpt, முழு mtlen)
{
	கட்டமைப்பு statfs *mp;
        முழு    len, n;
        மாறிலி  எழுத்து   *s1;
	எழுத்து	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        எனில் (n == 0)
		திரும்பு errno;

        len = அளவு(_PATH_DEV) - 1;
        s1 = file;
        எனில் (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        போது (--n >= 0) {
                s2 = mp->f_mntfromname;
                எனில் (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                எனில் (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			நிறுத்து;
		}
                ++mp;
	}
	எனில் (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	திரும்பு 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
நிலையான முழு is_swap_device(மாறிலி எழுத்து *file)
{
	FILE		*f;
	எழுத்து		buf[1024], *cp;
	dev_t		file_dev;
	கட்டமைப்பு stat	st_buf;
	முழு		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	எனில் ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	எனில் (!(f = fopen("/proc/swaps", "r")))
		திரும்பு 0;
	/* Skip the first line */
	எனில் (!fgets(buf, அளவு(buf), f))
		செல் leave;
	எனில் (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		செல் valid_first_வரி;

	போது (fgets(buf, அளவு(buf), f)) {
valid_first_வரி:
		எனில் ((cp = strchr(buf, ' ')) != வெற்று)
			*cp = 0;
		எனில் ((cp = strchr(buf, '\t')) != வெற்று)
			*cp = 0;
		எனில் (strcmp(buf, file) == 0) {
			ret++;
			நிறுத்து;
		}
#ifndef __GNU__
		எனில் (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			நிறுத்து;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	திரும்பு ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_check_mount_point(மாறிலி எழுத்து *device, முழு *mount_flags,
				  எழுத்து *mtpt, முழு mtlen)
{
	errcode_t	retval = 0;
	முழு 		busy = 0;

	எனில் (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		எனில் (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		திரும்பு 0;
	}
	எனில் (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		எனில் (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		திரும்பு 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		கட்டமைப்பு stat st_buf;

		எனில் (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			முழு fd = open(device, O_RDONLY | O_EXCL);

			எனில் (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				திரும்பு 0;
			} இல்லையெனில் எனில் (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	எனில் (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		எனில் (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} இல்லையெனில் {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#இல்லையெனில்
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#இல்லையெனில்
#எனில் defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	எனில் (retval)
		திரும்பு retval;

	எனில் (busy)
		*mount_flags |= EXT2_MF_BUSY;

	திரும்பு 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(மாறிலி எழுத்து *file, முழு *mount_flags)
{
	திரும்பு ext2fs_check_mount_point(file, mount_flags, வெற்று, 0);
}

#ifdef DEBUG
முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	முழு	retval, mount_flags;
	எழுத்து	mntpt[80];

	எனில் (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, அளவு(mntpt));
	எனில் (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	எனில் (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	எனில் (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	எனில் (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	எனில் (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	எனில் (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	எனில் (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
