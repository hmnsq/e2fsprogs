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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#অন্তর্ভাৱ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#অন্তর্ভাৱ <linux/loop.h>
#অন্তর্ভাৱ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#অন্তর্ভাৱ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#অন্তর্ভাৱ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#অন্তর্ভাৱ <paths.h>
#অন্তর্ভাৱ <sys/param.h>
#অন্তর্ভাৱ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sys/stat.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#অন্তর্ভাৱ <sys/sysmacros.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
স্থির পূর্ণাঙ্ক check_loop_mounted(ধ্রুৱ ৱর্ণ *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#যদি defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	সংরচনা loop_info64 loopinfo = {0, };
	পূর্ণাঙ্ক loop_fd, ret;

	যদি (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		যদি (loop_fd < 0)
			ঘূৰাই -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		যদি (ret < 0)
			ঘূৰাই -1;

		যদি (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ঘূৰাই 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ঘূৰাই 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
স্থির errcode_t check_mntent_file(ধ্রুৱ ৱর্ণ *mtab_file, ধ্রুৱ ৱর্ণ *file,
				   পূর্ণাঙ্ক *mount_flags, ৱর্ণ *mtpt, পূর্ণাঙ্ক mtlen)
{
	সংরচনা mntent 	*mnt;
	সংরচনা stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	পূর্ণাঙ্ক		fd;

	*mount_flags = 0;

	যদি ((f = setmntent (mtab_file, "r")) == রিক্ত) {
		যদি (errno == ENOENT) {
			যদি (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ঘূৰাই 0;
			অন্যথা
				ঘূৰাই EXT2_ET_NO_MTAB_FILE;
		}
		ঘূৰাই errno;
	}
	যদি (stat(file, &st_buf) == 0) {
		যদি (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} অন্যথা {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	যেতিয়ালৈ ((mnt = getmntent (f)) != রিক্ত) {
		যদি (mnt->mnt_fsname[0] != '/')
			অনুৱর্ত;
		যদি (strcmp(file, mnt->mnt_fsname) == 0) {
			যদি (stat(mnt->mnt_dir, &st_buf) != 0)
				অনুৱর্ত;
			যদি (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				অনুৱর্ত;
			}
			ভঙ্গ;
		}
		যদি (stat(mnt->mnt_fsname, &st_buf) == 0) {
			যদি (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				যদি (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					যদি (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						অনুৱর্ত;
					যদি (file_rdev == dir_st_buf.st_dev)
						ভঙ্গ;
				}
				যদি (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ভঙ্গ;
#endif	/* __GNU__ */
			} অন্যথা {
				যদি (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ভঙ্গ;
			}
		}
	}

	যদি (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		যদি (file_rdev && stat("/", &st_buf) == 0) {
			যদি (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				যদি (mtpt)
					strncpy(mtpt, "/", mtlen);
				গচ্ছ is_মূল;
			}
		}
#endif	/* __GNU__ */
		গচ্ছ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	যদি (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	যদি (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	যদি (!strcmp(mnt->mnt_dir, "/")) {
is_মূল:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		যদি (fd < 0) {
			যদি (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} অন্যথা
			close(fd);
		(শূন্য) unlink(TEST_FILE);
	}

	যদি (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ঘূৰাই retval;
}

স্থির errcode_t check_mntent(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *mount_flags,
			      ৱর্ণ *mtpt, পূর্ণাঙ্ক mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	যদি (retval == 0)
		ঘূৰাই 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	যদি (retval == 0)
		ঘূৰাই 0;
#endif /* __linux__ */
#যদি defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ঘূৰাই retval;
#অন্যথা
	*mount_flags = 0;
	ঘূৰাই 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#অন্যথা
#যদি defined(HAVE_GETMNTINFO)

স্থির errcode_t check_getmntinfo(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *mount_flags,
				  ৱর্ণ *mtpt, পূর্ণাঙ্ক mtlen)
{
	সংরচনা statfs *mp;
        পূর্ণাঙ্ক    len, n;
        ধ্রুৱ  ৱর্ণ   *s1;
	ৱর্ণ	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        যদি (n == 0)
		ঘূৰাই errno;

        len = পরিমাণ(_PATH_DEV) - 1;
        s1 = file;
        যদি (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        যেতিয়ালৈ (--n >= 0) {
                s2 = mp->f_mntfromname;
                যদি (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                যদি (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ভঙ্গ;
		}
                ++mp;
	}
	যদি (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ঘূৰাই 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
স্থির পূর্ণাঙ্ক is_swap_device(ধ্রুৱ ৱর্ণ *file)
{
	FILE		*f;
	ৱর্ণ		buf[1024], *cp;
	dev_t		file_dev;
	সংরচনা stat	st_buf;
	পূর্ণাঙ্ক		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	যদি ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	যদি (!(f = fopen("/proc/swaps", "r")))
		ঘূৰাই 0;
	/* Skip the first line */
	যদি (!fgets(buf, পরিমাণ(buf), f))
		গচ্ছ leave;
	যদি (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		গচ্ছ valid_first_সাৰি;

	যেতিয়ালৈ (fgets(buf, পরিমাণ(buf), f)) {
valid_first_সাৰি:
		যদি ((cp = strchr(buf, ' ')) != রিক্ত)
			*cp = 0;
		যদি ((cp = strchr(buf, '\t')) != রিক্ত)
			*cp = 0;
		যদি (strcmp(buf, file) == 0) {
			ret++;
			ভঙ্গ;
		}
#ifndef __GNU__
		যদি (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ভঙ্গ;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ঘূৰাই ret;
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
errcode_t ext2fs_check_mount_point(ধ্রুৱ ৱর্ণ *device, পূর্ণাঙ্ক *mount_flags,
				  ৱর্ণ *mtpt, পূর্ণাঙ্ক mtlen)
{
	errcode_t	retval = 0;
	পূর্ণাঙ্ক 		busy = 0;

	যদি (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		যদি (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ঘূৰাই 0;
	}
	যদি (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		যদি (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ঘূৰাই 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		সংরচনা stat st_buf;

		যদি (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			পূর্ণাঙ্ক fd = open(device, O_RDONLY | O_EXCL);

			যদি (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ঘূৰাই 0;
			} অন্যথা যদি (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	যদি (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		যদি (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} অন্যথা {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#অন্যথা
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#অন্যথা
#যদি defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	যদি (retval)
		ঘূৰাই retval;

	যদি (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ঘূৰাই 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *mount_flags)
{
	ঘূৰাই ext2fs_check_mount_point(file, mount_flags, রিক্ত, 0);
}

#ifdef DEBUG
পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
	পূর্ণাঙ্ক	retval, mount_flags;
	ৱর্ণ	mntpt[80];

	যদি (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, পরিমাণ(mntpt));
	যদি (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	যদি (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	যদি (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	যদি (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	যদি (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	যদি (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	যদি (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
