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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#شامل <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#شامل <linux/loop.h>
#شامل <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#شامل <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#شامل <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#شامل <paths.h>
#شامل <sys/param.h>
#شامل <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#شامل <string.h>
#شامل <sys/stat.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#شامل <sys/sysmacros.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ایستا صحیح check_loop_mounted(ثابت نویسه *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#اگر defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ساختار loop_info64 loopinfo = {0, };
	صحیح loop_fd, ret;

	اگر (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		اگر (loop_fd < 0)
			بازگشت -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		اگر (ret < 0)
			بازگشت -1;

		اگر (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			بازگشت 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	بازگشت 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ایستا errcode_t check_mntent_file(ثابت نویسه *mtab_file, ثابت نویسه *file,
				   صحیح *mount_flags, نویسه *mtpt, صحیح mtlen)
{
	ساختار mntent 	*mnt;
	ساختار stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	صحیح		fd;

	*mount_flags = 0;

	اگر ((f = setmntent (mtab_file, "r")) == پوچ) {
		اگر (errno == ENOENT) {
			اگر (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				بازگشت 0;
			وگرنه
				بازگشت EXT2_ET_NO_MTAB_FILE;
		}
		بازگشت errno;
	}
	اگر (stat(file, &st_buf) == 0) {
		اگر (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} وگرنه {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	وقتی ((mnt = getmntent (f)) != پوچ) {
		اگر (mnt->mnt_fsname[0] != '/')
			ادامه;
		اگر (strcmp(file, mnt->mnt_fsname) == 0) {
			اگر (stat(mnt->mnt_dir, &st_buf) != 0)
				ادامه;
			اگر (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				ادامه;
			}
			بشکن;
		}
		اگر (stat(mnt->mnt_fsname, &st_buf) == 0) {
			اگر (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				اگر (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					اگر (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ادامه;
					اگر (file_rdev == dir_st_buf.st_dev)
						بشکن;
				}
				اگر (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					بشکن;
#endif	/* __GNU__ */
			} وگرنه {
				اگر (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					بشکن;
			}
		}
	}

	اگر (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		اگر (file_rdev && stat("/", &st_buf) == 0) {
			اگر (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				اگر (mtpt)
					strncpy(mtpt, "/", mtlen);
				برو is_ریشه;
			}
		}
#endif	/* __GNU__ */
		برو errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	اگر (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	اگر (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	اگر (!strcmp(mnt->mnt_dir, "/")) {
is_ریشه:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		اگر (fd < 0) {
			اگر (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} وگرنه
			close(fd);
		(تهی) unlink(TEST_FILE);
	}

	اگر (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	بازگشت retval;
}

ایستا errcode_t check_mntent(ثابت نویسه *file, صحیح *mount_flags,
			      نویسه *mtpt, صحیح mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	اگر (retval == 0)
		بازگشت 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	اگر (retval == 0)
		بازگشت 0;
#endif /* __linux__ */
#اگر defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	بازگشت retval;
#وگرنه
	*mount_flags = 0;
	بازگشت 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#وگرنه
#اگر defined(HAVE_GETMNTINFO)

ایستا errcode_t check_getmntinfo(ثابت نویسه *file, صحیح *mount_flags,
				  نویسه *mtpt, صحیح mtlen)
{
	ساختار statfs *mp;
        صحیح    len, n;
        ثابت  نویسه   *s1;
	نویسه	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        اگر (n == 0)
		بازگشت errno;

        len = اندازه(_PATH_DEV) - 1;
        s1 = file;
        اگر (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        وقتی (--n >= 0) {
                s2 = mp->f_mntfromname;
                اگر (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                اگر (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			بشکن;
		}
                ++mp;
	}
	اگر (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	بازگشت 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ایستا صحیح is_swap_device(ثابت نویسه *file)
{
	FILE		*f;
	نویسه		buf[1024], *cp;
	dev_t		file_dev;
	ساختار stat	st_buf;
	صحیح		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	اگر ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	اگر (!(f = fopen("/proc/swaps", "r")))
		بازگشت 0;
	/* Skip the first line */
	اگر (!fgets(buf, اندازه(buf), f))
		برو leave;
	اگر (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		برو valid_first_سطر;

	وقتی (fgets(buf, اندازه(buf), f)) {
valid_first_سطر:
		اگر ((cp = strchr(buf, ' ')) != پوچ)
			*cp = 0;
		اگر ((cp = strchr(buf, '\t')) != پوچ)
			*cp = 0;
		اگر (strcmp(buf, file) == 0) {
			ret++;
			بشکن;
		}
#ifndef __GNU__
		اگر (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			بشکن;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	بازگشت ret;
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
errcode_t ext2fs_check_mount_point(ثابت نویسه *device, صحیح *mount_flags,
				  نویسه *mtpt, صحیح mtlen)
{
	errcode_t	retval = 0;
	صحیح 		busy = 0;

	اگر (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		اگر (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		بازگشت 0;
	}
	اگر (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		اگر (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		بازگشت 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		ساختار stat st_buf;

		اگر (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			صحیح fd = open(device, O_RDONLY | O_EXCL);

			اگر (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				بازگشت 0;
			} وگرنه اگر (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	اگر (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		اگر (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} وگرنه {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#وگرنه
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#وگرنه
#اگر defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	اگر (retval)
		بازگشت retval;

	اگر (busy)
		*mount_flags |= EXT2_MF_BUSY;

	بازگشت 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ثابت نویسه *file, صحیح *mount_flags)
{
	بازگشت ext2fs_check_mount_point(file, mount_flags, پوچ, 0);
}

#ifdef DEBUG
صحیح اصلی(صحیح argc, نویسه **argv)
{
	صحیح	retval, mount_flags;
	نویسه	mntpt[80];

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, اندازه(mntpt));
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	اگر (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	اگر (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
