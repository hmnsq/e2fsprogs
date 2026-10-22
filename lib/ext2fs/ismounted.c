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

#ضمن "config.h"
#ضمن <stdio.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#ضمن <linux/loop.h>
#ضمن <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#ضمن <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#ضمن <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#ضمن <paths.h>
#ضمن <sys/param.h>
#ضمن <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#ضمن <string.h>
#ضمن <sys/stat.h>
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ساكن صحيح check_loop_mounted(ثابت حرف *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ܐܢ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	بنية loop_info64 loopinfo = {0, };
	صحيح loop_fd, ret;

	ܐܢ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ܐܢ (loop_fd < 0)
			ܦܢܐ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ܐܢ (ret < 0)
			ܦܢܐ -1;

		ܐܢ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ܦܢܐ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ܦܢܐ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ساكن errcode_t check_mntent_file(ثابت حرف *mtab_file, ثابت حرف *file,
				   صحيح *mount_flags, حرف *mtpt, صحيح mtlen)
{
	بنية mntent 	*mnt;
	بنية stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	صحيح		fd;

	*mount_flags = 0;

	ܐܢ ((f = setmntent (mtab_file, "r")) == عدم) {
		ܐܢ (errno == ENOENT) {
			ܐܢ (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ܦܢܐ 0;
			ܐܚܪܢܐ
				ܦܢܐ EXT2_ET_NO_MTAB_FILE;
		}
		ܦܢܐ errno;
	}
	ܐܢ (stat(file, &st_buf) == 0) {
		ܐܢ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} ܐܚܪܢܐ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ܟܡܐ ((mnt = getmntent (f)) != عدم) {
		ܐܢ (mnt->mnt_fsname[0] != '/')
			استمر;
		ܐܢ (strcmp(file, mnt->mnt_fsname) == 0) {
			ܐܢ (stat(mnt->mnt_dir, &st_buf) != 0)
				استمر;
			ܐܢ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				استمر;
			}
			اكسر;
		}
		ܐܢ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ܐܢ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				ܐܢ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ܐܢ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						استمر;
					ܐܢ (file_rdev == dir_st_buf.st_dev)
						اكسر;
				}
				ܐܢ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					اكسر;
#endif	/* __GNU__ */
			} ܐܚܪܢܐ {
				ܐܢ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					اكسر;
			}
		}
	}

	ܐܢ (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ܐܢ (file_rdev && stat("/", &st_buf) == 0) {
			ܐܢ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ܐܢ (mtpt)
					strncpy(mtpt, "/", mtlen);
				اذهب is_root;
			}
		}
#endif	/* __GNU__ */
		اذهب errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	ܐܢ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	ܐܢ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ܐܢ (!strcmp(mnt->mnt_dir, "/")) {
is_root:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ܐܢ (fd < 0) {
			ܐܢ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ܐܚܪܢܐ
			close(fd);
		(فراغ) unlink(TEST_FILE);
	}

	ܐܢ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ܦܢܐ retval;
}

ساكن errcode_t check_mntent(ثابت حرف *file, صحيح *mount_flags,
			      حرف *mtpt, صحيح mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ܐܢ (retval == 0)
		ܦܢܐ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ܐܢ (retval == 0)
		ܦܢܐ 0;
#endif /* __linux__ */
#ܐܢ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ܦܢܐ retval;
#ܐܚܪܢܐ
	*mount_flags = 0;
	ܦܢܐ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ܐܚܪܢܐ
#ܐܢ defined(HAVE_GETMNTINFO)

ساكن errcode_t check_getmntinfo(ثابت حرف *file, صحيح *mount_flags,
				  حرف *mtpt, صحيح mtlen)
{
	بنية statfs *mp;
        صحيح    len, n;
        ثابت  حرف   *s1;
	حرف	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ܐܢ (n == 0)
		ܦܢܐ errno;

        len = حجم(_PATH_DEV) - 1;
        s1 = file;
        ܐܢ (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ܟܡܐ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ܐܢ (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ܐܢ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			اكسر;
		}
                ++mp;
	}
	ܐܢ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ܦܢܐ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ساكن صحيح is_swap_device(ثابت حرف *file)
{
	FILE		*f;
	حرف		buf[1024], *cp;
	dev_t		file_dev;
	بنية stat	st_buf;
	صحيح		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ܐܢ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	ܐܢ (!(f = fopen("/proc/swaps", "r")))
		ܦܢܐ 0;
	/* Skip the first line */
	ܐܢ (!fgets(buf, حجم(buf), f))
		اذهب leave;
	ܐܢ (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		اذهب valid_first_line;

	ܟܡܐ (fgets(buf, حجم(buf), f)) {
valid_first_line:
		ܐܢ ((cp = strchr(buf, ' ')) != عدم)
			*cp = 0;
		ܐܢ ((cp = strchr(buf, '\t')) != عدم)
			*cp = 0;
		ܐܢ (strcmp(buf, file) == 0) {
			ret++;
			اكسر;
		}
#ifndef __GNU__
		ܐܢ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			اكسر;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ܦܢܐ ret;
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
errcode_t ext2fs_check_mount_point(ثابت حرف *device, صحيح *mount_flags,
				  حرف *mtpt, صحيح mtlen)
{
	errcode_t	retval = 0;
	صحيح 		busy = 0;

	ܐܢ (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ܐܢ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ܦܢܐ 0;
	}
	ܐܢ (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ܐܢ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ܦܢܐ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		بنية stat st_buf;

		ܐܢ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			صحيح fd = open(device, O_RDONLY | O_EXCL);

			ܐܢ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ܦܢܐ 0;
			} ܐܚܪܢܐ ܐܢ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	ܐܢ (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		ܐܢ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ܐܚܪܢܐ {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ܐܚܪܢܐ
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ܐܚܪܢܐ
#ܐܢ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	ܐܢ (retval)
		ܦܢܐ retval;

	ܐܢ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ܦܢܐ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ثابت حرف *file, صحيح *mount_flags)
{
	ܦܢܐ ext2fs_check_mount_point(file, mount_flags, عدم, 0);
}

#ifdef DEBUG
صحيح main(صحيح argc, حرف **argv)
{
	صحيح	retval, mount_flags;
	حرف	mntpt[80];

	ܐܢ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, حجم(mntpt));
	ܐܢ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	ܐܢ (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	ܐܢ (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	ܐܢ (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	ܐܢ (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	ܐܢ (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	ܐܢ (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
