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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BSD_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BSD_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _DEFAULT_SOURCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_FD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/fd.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_LOOP_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/loop.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_MAJOR_H
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/major.h>
#ਜੇ_ਅੰਤ /* HAVE_LINUX_MAJOR_H */
#ਜੇ_ਅੰਤ /* HAVE_LINUX_LOOP_H */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MNTENT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <mntent.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETMNTINFO
#ਅਨ੍ਤਰ੍ਭਾਵ <paths.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/param.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਜੇ_ਅੰਤ /* HAVE_GETMNTINFO */
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_SYSMACROS_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ਸਥਿਰ ਪੂਰਨ check_loop_mounted(ਅਚਲ ਅੱਖਰ *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ਜੇ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ਬਣਤਰ loop_info64 loopinfo = {0, };
	ਪੂਰਨ loop_fd, ret;

	ਜੇ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ਜੇ (loop_fd < 0)
			ਵਾਪਸ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ਜੇ (ret < 0)
			ਵਾਪਸ -1;

		ਜੇ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ਵਾਪਸ 1;
	}
#ਜੇ_ਅੰਤ /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ਵਾਪਸ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ਸਥਿਰ errcode_t check_mntent_file(ਅਚਲ ਅੱਖਰ *mtab_file, ਅਚਲ ਅੱਖਰ *file,
				   ਪੂਰਨ *mount_flags, ਅੱਖਰ *mtpt, ਪੂਰਨ mtlen)
{
	ਬਣਤਰ mntent 	*mnt;
	ਬਣਤਰ stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ਪੂਰਨ		fd;

	*mount_flags = 0;

	ਜੇ ((f = setmntent (mtab_file, "r")) == ਨਿਰਾ) {
		ਜੇ (errno == ENOENT) {
			ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("EXT2FS_NO_MTAB_OK"))
				ਵਾਪਸ 0;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ EXT2_ET_NO_MTAB_FILE;
		}
		ਵਾਪਸ errno;
	}
	ਜੇ (stat(file, &st_buf) == 0) {
		ਜੇ (ext2fsP_is_disk_device(st_buf.st_ਬਹੁਲਕ)) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#ਜੇ_ਅੰਤ	/* __GNU__ */
		} ਨਹੀਂਤਾਂ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ਜਦੋਂ ((mnt = getmntent (f)) != ਨਿਰਾ) {
		ਜੇ (mnt->mnt_fsname[0] != '/')
			ਜਾਰੀ;
		ਜੇ (strcmp(file, mnt->mnt_fsname) == 0) {
			ਜੇ (stat(mnt->mnt_dir, &st_buf) != 0)
				ਜਾਰੀ;
			ਜੇ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
				ਲਿਖੋ("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#ਜੇ_ਅੰਤ /* DEBUG */
				ਜਾਰੀ;
			}
			ਰੋਕੋ;
		}
		ਜੇ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ਜੇ (ext2fsP_is_disk_device(st_buf.st_ਬਹੁਲਕ)) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNU__
				ਜੇ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ਜੇ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ਜਾਰੀ;
					ਜੇ (file_rdev == dir_st_buf.st_dev)
						ਰੋਕੋ;
				}
				ਜੇ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ਰੋਕੋ;
#ਜੇ_ਅੰਤ	/* __GNU__ */
			} ਨਹੀਂਤਾਂ {
				ਜੇ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ਰੋਕੋ;
			}
		}
	}

	ਜੇ (mnt == 0) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ਜੇ (file_rdev && stat("/", &st_buf) == 0) {
			ਜੇ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ਜੇ (mtpt)
					strncpy(mtpt, "/", mtlen);
				ਜਾਓ is_ਜੜ੍ਹ;
			}
		}
#ਜੇ_ਅੰਤ	/* __GNU__ */
		ਜਾਓ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MNTOPT_RO
	/* Check to see if the ro option is set */
	ਜੇ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#ਜੇ_ਅੰਤ

	ਜੇ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ਜੇ (!strcmp(mnt->mnt_dir, "/")) {
is_ਜੜ੍ਹ:
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ਜੇ (fd < 0) {
			ਜੇ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ਨਹੀਂਤਾਂ
			close(fd);
		(ਖਾਲੀ) unlink(TEST_FILE);
	}

	ਜੇ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t check_mntent(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *mount_flags,
			      ਅੱਖਰ *mtpt, ਪੂਰਨ mtlen)
{
	errcode_t	retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ਜੇ (retval == 0)
		ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ /* DEBUG */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ਜੇ (retval == 0)
		ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ /* __linux__ */
#ਜੇ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MOUNTED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MOUNTED _PATH_MOUNTED
#ਜੇ_ਅੰਤ /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	*mount_flags = 0;
	ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ਨਹੀਂਤਾਂ
#ਜੇ defined(HAVE_GETMNTINFO)

ਸਥਿਰ errcode_t check_getmntinfo(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *mount_flags,
				  ਅੱਖਰ *mtpt, ਪੂਰਨ mtlen)
{
	ਬਣਤਰ statfs *mp;
        ਪੂਰਨ    len, n;
        ਅਚਲ  ਅੱਖਰ   *s1;
	ਅੱਖਰ	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ਜੇ (n == 0)
		ਵਾਪਸ errno;

        len = ਆਕਾਰ(_PATH_DEV) - 1;
        s1 = file;
        ਜੇ (ਲੜੀ_ਤੁਲਨਾ(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ਜਦੋਂ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ਜੇ (ਲੜੀ_ਤੁਲਨਾ(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ਜੇ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ਰੋਕੋ;
		}
                ++mp;
	}
	ਜੇ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* HAVE_GETMNTINFO */
#ਜੇ_ਅੰਤ /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ਸਥਿਰ ਪੂਰਨ is_ਅਦਲਾ_ਬਦਲੀ_device(ਅਚਲ ਅੱਖਰ *file)
{
	FILE		*f;
	ਅੱਖਰ		buf[1024], *cp;
	dev_t		file_dev;
	ਬਣਤਰ stat	st_buf;
	ਪੂਰਨ		ret = 0;

	file_dev = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ਜੇ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_ਬਹੁਲਕ))
		file_dev = st_buf.st_rdev;
#ਜੇ_ਅੰਤ	/* __GNU__ */

	ਜੇ (!(f = fopen("/proc/swaps", "r")))
		ਵਾਪਸ 0;
	/* Skip the first line */
	ਜੇ (!fgets(buf, ਆਕਾਰ(buf), f))
		ਜਾਓ leave;
	ਜੇ (*buf && ਲੜੀ_ਤੁਲਨਾ(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		ਜਾਓ valid_ਪਹਿਲਾ_ਸਤਰ;

	ਜਦੋਂ (fgets(buf, ਆਕਾਰ(buf), f)) {
valid_ਪਹਿਲਾ_ਸਤਰ:
		ਜੇ ((cp = strchr(buf, ' ')) != ਨਿਰਾ)
			*cp = 0;
		ਜੇ ((cp = strchr(buf, '\t')) != ਨਿਰਾ)
			*cp = 0;
		ਜੇ (strcmp(buf, file) == 0) {
			ret++;
			ਰੋਕੋ;
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNU__
		ਜੇ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_ਬਹੁਲਕ) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ਰੋਕੋ;
		}
#ਜੇ_ਅੰਤ 	/* __GNU__ */
	}

leave:
	fclose(f);
	ਵਾਪਸ ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
errcode_t ext2fs_check_mount_ਬਿੰਦੂ(ਅਚਲ ਅੱਖਰ *device, ਪੂਰਨ *mount_flags,
				  ਅੱਖਰ *mtpt, ਪੂਰਨ mtlen)
{
	errcode_t	retval = 0;
	ਪੂਰਨ 		busy = 0;

	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ਵਾਪਸ 0;
	}
	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ਵਾਪਸ 0;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__ /* This only works on Linux 2.6+ systems */
	{
		ਬਣਤਰ stat st_buf;

		ਜੇ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_ਬਹੁਲਕ)) {
			ਪੂਰਨ fd = open(device, O_RDONLY | O_EXCL);

			ਜੇ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ਵਾਪਸ 0;
			} ਨਹੀਂਤਾਂ ਜੇ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#ਜੇ_ਅੰਤ

	ਜੇ (is_ਅਦਲਾ_ਬਦਲੀ_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_ਅਦਲਾ_ਬਦਲੀ;
		ਜੇ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ਨਹੀਂਤਾਂ {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ਨਹੀਂਤਾਂ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ਨਹੀਂਤਾਂ
#ਜੇ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#ਜੇ_ਅੰਤ
		*mount_flags = 0;
#ਜੇ_ਅੰਤ /* HAVE_GETMNTINFO */
#ਜੇ_ਅੰਤ /* HAVE_SETMNTENT */
	}
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ਵਾਪਸ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *mount_flags)
{
	ਵਾਪਸ ext2fs_check_mount_ਬਿੰਦੂ(file, mount_flags, ਨਿਰਾ, 0);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ	retval, mount_flags;
	ਅੱਖਰ	mntpt[80];

	ਜੇ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	ਜੋੜੋ_error_ਸਾਰਣੀ(&et_ext2_error_ਸਾਰਣੀ);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_ਬਿੰਦੂ(argv[1], &mount_flags,
					  mntpt, ਆਕਾਰ(mntpt));
	ਜੇ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("Device %s reports flags %02x\n", argv[1], mount_flags);
	ਜੇ (mount_flags & EXT2_MF_BUSY)
		ਲਿਖੋ("\t%s is apparently in use.\n", argv[1]);
	ਜੇ (mount_flags & EXT2_MF_MOUNTED)
		ਲਿਖੋ("\t%s is mounted.\n", argv[1]);
	ਜੇ (mount_flags & EXT2_MF_ਅਦਲਾ_ਬਦਲੀ)
		ਲਿਖੋ("\t%s is a swap device.\n", argv[1]);
	ਜੇ (mount_flags & EXT2_MF_READONLY)
		ਲਿਖੋ("\t%s is read-only.\n", argv[1]);
	ਜੇ (mount_flags & EXT2_MF_ISROOT)
		ਲਿਖੋ("\t%s is the root filesystem.\n", argv[1]);
	ਜੇ (mntpt[0])
		ਲਿਖੋ("\t%s is mounted on %s.\n", argv[1], mntpt);
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}
#ਜੇ_ਅੰਤ /* DEBUG */
