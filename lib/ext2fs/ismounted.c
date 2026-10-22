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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#ಅನ್ತರ್ಭಾವ <linux/loop.h>
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#ಅನ್ತರ್ಭಾವ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#ಅನ್ತರ್ಭಾವ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#ಅನ್ತರ್ಭಾವ <paths.h>
#ಅನ್ತರ್ಭಾವ <sys/param.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_loop_mounted(ಧ್ರುವ ವರ್ಣ *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ಯದಿ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ಸಂರಚನಾ loop_info64 loopinfo = {0, };
	ಪೂರ್ಣಾಙ್ಕ loop_fd, ret;

	ಯದಿ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ಯದಿ (loop_fd < 0)
			ಪ್ರತ್ಯಾಗಮ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ಯದಿ (ret < 0)
			ಪ್ರತ್ಯಾಗಮ -1;

		ಯದಿ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ಪ್ರತ್ಯಾಗಮ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ಸ್ಥಿರ errcode_t check_mntent_file(ಧ್ರುವ ವರ್ಣ *mtab_file, ಧ್ರುವ ವರ್ಣ *file,
				   ಪೂರ್ಣಾಙ್ಕ *mount_flags, ವರ್ಣ *mtpt, ಪೂರ್ಣಾಙ್ಕ mtlen)
{
	ಸಂರಚನಾ mntent 	*mnt;
	ಸಂರಚನಾ stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ಪೂರ್ಣಾಙ್ಕ		fd;

	*mount_flags = 0;

	ಯದಿ ((f = setmntent (mtab_file, "r")) == ರಿಕ್ತ) {
		ಯದಿ (errno == ENOENT) {
			ಯದಿ (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ಪ್ರತ್ಯಾಗಮ 0;
			ಅನ್ಯಥಾ
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MTAB_FILE;
		}
		ಪ್ರತ್ಯಾಗಮ errno;
	}
	ಯದಿ (stat(file, &st_buf) == 0) {
		ಯದಿ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} ಅನ್ಯಥಾ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ಯಾವತ್ ((mnt = getmntent (f)) != ರಿಕ್ತ) {
		ಯದಿ (mnt->mnt_fsname[0] != '/')
			ಅನುವರ್ತ;
		ಯದಿ (strcmp(file, mnt->mnt_fsname) == 0) {
			ಯದಿ (stat(mnt->mnt_dir, &st_buf) != 0)
				ಅನುವರ್ತ;
			ಯದಿ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				ಅನುವರ್ತ;
			}
			ಭಙ್ಗ;
		}
		ಯದಿ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ಯದಿ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				ಯದಿ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ಯದಿ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ಅನುವರ್ತ;
					ಯದಿ (file_rdev == dir_st_buf.st_dev)
						ಭಙ್ಗ;
				}
				ಯದಿ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ಭಙ್ಗ;
#endif	/* __GNU__ */
			} ಅನ್ಯಥಾ {
				ಯದಿ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ಭಙ್ಗ;
			}
		}
	}

	ಯದಿ (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ಯದಿ (file_rdev && stat("/", &st_buf) == 0) {
			ಯದಿ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ಯದಿ (mtpt)
					strncpy(mtpt, "/", mtlen);
				ಗಚ್ಛ is_ಬೇರು;
			}
		}
#endif	/* __GNU__ */
		ಗಚ್ಛ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	ಯದಿ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	ಯದಿ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ಯದಿ (!strcmp(mnt->mnt_dir, "/")) {
is_ಬೇರು:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ಯದಿ (fd < 0) {
			ಯದಿ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ಅನ್ಯಥಾ
			close(fd);
		(ಶೂನ್ಯ) unlink(TEST_FILE);
	}

	ಯದಿ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t check_mntent(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *mount_flags,
			      ವರ್ಣ *mtpt, ಪೂರ್ಣಾಙ್ಕ mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ಯದಿ (retval == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ಯದಿ (retval == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
#endif /* __linux__ */
#ಯದಿ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ಪ್ರತ್ಯಾಗಮ retval;
#ಅನ್ಯಥಾ
	*mount_flags = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ಅನ್ಯಥಾ
#ಯದಿ defined(HAVE_GETMNTINFO)

ಸ್ಥಿರ errcode_t check_getmntinfo(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *mount_flags,
				  ವರ್ಣ *mtpt, ಪೂರ್ಣಾಙ್ಕ mtlen)
{
	ಸಂರಚನಾ statfs *mp;
        ಪೂರ್ಣಾಙ್ಕ    len, n;
        ಧ್ರುವ  ವರ್ಣ   *s1;
	ವರ್ಣ	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ಯದಿ (n == 0)
		ಪ್ರತ್ಯಾಗಮ errno;

        len = ಪರಿಮಾಣ(_PATH_DEV) - 1;
        s1 = file;
        ಯದಿ (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ಯಾವತ್ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ಯದಿ (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ಯದಿ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ಭಙ್ಗ;
		}
                ++mp;
	}
	ಯದಿ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ಪ್ರತ್ಯಾಗಮ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_swap_device(ಧ್ರುವ ವರ್ಣ *file)
{
	FILE		*f;
	ವರ್ಣ		buf[1024], *cp;
	dev_t		file_dev;
	ಸಂರಚನಾ stat	st_buf;
	ಪೂರ್ಣಾಙ್ಕ		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ಯದಿ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	ಯದಿ (!(f = fopen("/proc/swaps", "r")))
		ಪ್ರತ್ಯಾಗಮ 0;
	/* Skip the first line */
	ಯದಿ (!fgets(buf, ಪರಿಮಾಣ(buf), f))
		ಗಚ್ಛ leave;
	ಯದಿ (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		ಗಚ್ಛ valid_first_ಸಾಲು;

	ಯಾವತ್ (fgets(buf, ಪರಿಮಾಣ(buf), f)) {
valid_first_ಸಾಲು:
		ಯದಿ ((cp = strchr(buf, ' ')) != ರಿಕ್ತ)
			*cp = 0;
		ಯದಿ ((cp = strchr(buf, '\t')) != ರಿಕ್ತ)
			*cp = 0;
		ಯದಿ (strcmp(buf, file) == 0) {
			ret++;
			ಭಙ್ಗ;
		}
#ifndef __GNU__
		ಯದಿ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ಭಙ್ಗ;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ಪ್ರತ್ಯಾಗಮ ret;
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
errcode_t ext2fs_check_mount_point(ಧ್ರುವ ವರ್ಣ *device, ಪೂರ್ಣಾಙ್ಕ *mount_flags,
				  ವರ್ಣ *mtpt, ಪೂರ್ಣಾಙ್ಕ mtlen)
{
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಙ್ಕ 		busy = 0;

	ಯದಿ (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ಯದಿ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ಯದಿ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		ಸಂರಚನಾ stat st_buf;

		ಯದಿ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			ಪೂರ್ಣಾಙ್ಕ fd = open(device, O_RDONLY | O_EXCL);

			ಯದಿ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ಪ್ರತ್ಯಾಗಮ 0;
			} ಅನ್ಯಥಾ ಯದಿ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	ಯದಿ (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		ಯದಿ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ಅನ್ಯಥಾ {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ಅನ್ಯಥಾ
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ಅನ್ಯಥಾ
#ಯದಿ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *mount_flags)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_check_mount_point(file, mount_flags, ರಿಕ್ತ, 0);
}

#ifdef DEBUG
ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ	retval, mount_flags;
	ವರ್ಣ	mntpt[80];

	ಯದಿ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, ಪರಿಮಾಣ(mntpt));
	ಯದಿ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	ಯದಿ (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	ಯದಿ (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	ಯದಿ (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	ಯದಿ (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	ಯದಿ (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	ಯದಿ (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
