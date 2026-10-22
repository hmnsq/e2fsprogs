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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BSD_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು BSD_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _DEFAULT_SOURCE
#ವ್ಯಾಖ್ಯಾನಿಸು _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_FD_H
#ಅನ್ತರ್ಭಾವ <linux/fd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_LOOP_H
#ಅನ್ತರ್ಭಾವ <linux/loop.h>
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_MAJOR_H
#ಅನ್ತರ್ಭಾವ <linux/major.h>
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_LINUX_MAJOR_H */
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_LINUX_LOOP_H */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_MNTENT_H
#ಅನ್ತರ್ಭಾವ <mntent.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETMNTINFO
#ಅನ್ತರ್ಭಾವ <paths.h>
#ಅನ್ತರ್ಭಾವ <sys/param.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_GETMNTINFO */
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_SYSMACROS_H
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_loop_mounted(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ಆದರೆ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ರಚನೆ loop_info64 loopinfo = {0, };
	ಪೂರ್ಣಾಂಕ loop_fd, ret;

	ಆದರೆ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ಆದರೆ (loop_fd < 0)
			ಹಿಂತಿರುಗಿ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ಆದರೆ (ret < 0)
			ಹಿಂತಿರುಗಿ -1;

		ಆದರೆ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ಹಿಂತಿರುಗಿ 1;
	}
#ಆದರೆ_ಅಂತ್ಯ /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ಸ್ಥಿರ errcode_t check_mntent_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mtab_file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
				   ಪೂರ್ಣಾಂಕ *mount_flags, ಅಕ್ಷರ *mtpt, ಪೂರ್ಣಾಂಕ mtlen)
{
	ರಚನೆ mntent 	*mnt;
	ರಚನೆ stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ಪೂರ್ಣಾಂಕ		fd;

	*mount_flags = 0;

	ಆದರೆ ((f = setmntent (mtab_file, "r")) == ಖಾಲಿ) {
		ಆದರೆ (errno == ENOENT) {
			ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("EXT2FS_NO_MTAB_OK"))
				ಹಿಂತಿರುಗಿ 0;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ EXT2_ET_NO_MTAB_FILE;
		}
		ಹಿಂತಿರುಗಿ errno;
	}
	ಆದರೆ (stat(file, &st_buf) == 0) {
		ಆದರೆ (ext2fsP_is_disk_device(st_buf.st_ಬಹುಲಕ)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#ಆದರೆ_ಅಂತ್ಯ	/* __GNU__ */
		} ಇಲ್ಲದಿದ್ದರೆ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ಆಗ ((mnt = getmntent (f)) != ಖಾಲಿ) {
		ಆದರೆ (mnt->mnt_fsname[0] != '/')
			ಮುಂದುವರಿ;
		ಆದರೆ (strcmp(file, mnt->mnt_fsname) == 0) {
			ಆದರೆ (stat(mnt->mnt_dir, &st_buf) != 0)
				ಮುಂದುವರಿ;
			ಆದರೆ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
				ಬರೆ("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#ಆದರೆ_ಅಂತ್ಯ /* DEBUG */
				ಮುಂದುವರಿ;
			}
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ಆದರೆ (ext2fsP_is_disk_device(st_buf.st_ಬಹುಲಕ)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNU__
				ಆದರೆ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ಆದರೆ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ಮುಂದುವರಿ;
					ಆದರೆ (file_rdev == dir_st_buf.st_dev)
						ನಿಲ್ಲಿಸು;
				}
				ಆದರೆ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ	/* __GNU__ */
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಆದರೆ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ನಿಲ್ಲಿಸು;
			}
		}
	}

	ಆದರೆ (mnt == 0) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ಆದರೆ (file_rdev && stat("/", &st_buf) == 0) {
			ಆದರೆ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ಆದರೆ (mtpt)
					strncpy(mtpt, "/", mtlen);
				ಹೋಗು is_ಬೇರು;
			}
		}
#ಆದರೆ_ಅಂತ್ಯ	/* __GNU__ */
		ಹೋಗು errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ MNTOPT_RO
	/* Check to see if the ro option is set */
	ಆದರೆ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ಆದರೆ (!strcmp(mnt->mnt_dir, "/")) {
is_ಬೇರು:
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ಆದರೆ (fd < 0) {
			ಆದರೆ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ಇಲ್ಲದಿದ್ದರೆ
			close(fd);
		(ಶೂನ್ಯ) unlink(TEST_FILE);
	}

	ಆದರೆ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t check_mntent(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *mount_flags,
			      ಅಕ್ಷರ *mtpt, ಪೂರ್ಣಾಂಕ mtlen)
{
	errcode_t	retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ಆದರೆ (retval == 0)
		ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ /* DEBUG */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ಆದರೆ (retval == 0)
		ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ /* __linux__ */
#ಆದರೆ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNTED
#ವ್ಯಾಖ್ಯಾನಿಸು MOUNTED _PATH_MOUNTED
#ಆದರೆ_ಅಂತ್ಯ /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	*mount_flags = 0;
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(HAVE_GETMNTINFO)

ಸ್ಥಿರ errcode_t check_getmntinfo(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *mount_flags,
				  ಅಕ್ಷರ *mtpt, ಪೂರ್ಣಾಂಕ mtlen)
{
	ರಚನೆ statfs *mp;
        ಪೂರ್ಣಾಂಕ    len, n;
        ಸ್ಥಿರಾಂಕ  ಅಕ್ಷರ   *s1;
	ಅಕ್ಷರ	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ಆದರೆ (n == 0)
		ಹಿಂತಿರುಗಿ errno;

        len = ಗಾತ್ರ(_PATH_DEV) - 1;
        s1 = file;
        ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ಆಗ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ಆದರೆ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ನಿಲ್ಲಿಸು;
		}
                ++mp;
	}
	ಆದರೆ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_GETMNTINFO */
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_ಅದಲುಬದಲು_device(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)
{
	FILE		*f;
	ಅಕ್ಷರ		buf[1024], *cp;
	dev_t		file_dev;
	ರಚನೆ stat	st_buf;
	ಪೂರ್ಣಾಂಕ		ret = 0;

	file_dev = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ಆದರೆ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_ಬಹುಲಕ))
		file_dev = st_buf.st_rdev;
#ಆದರೆ_ಅಂತ್ಯ	/* __GNU__ */

	ಆದರೆ (!(f = fopen("/proc/swaps", "r")))
		ಹಿಂತಿರುಗಿ 0;
	/* Skip the first line */
	ಆದರೆ (!fgets(buf, ಗಾತ್ರ(buf), f))
		ಹೋಗು leave;
	ಆದರೆ (*buf && ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		ಹೋಗು valid_ಮೊದಲ_ಸಾಲು;

	ಆಗ (fgets(buf, ಗಾತ್ರ(buf), f)) {
valid_ಮೊದಲ_ಸಾಲು:
		ಆದರೆ ((cp = strchr(buf, ' ')) != ಖಾಲಿ)
			*cp = 0;
		ಆದರೆ ((cp = strchr(buf, '\t')) != ಖಾಲಿ)
			*cp = 0;
		ಆದರೆ (strcmp(buf, file) == 0) {
			ret++;
			ನಿಲ್ಲಿಸು;
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNU__
		ಆದರೆ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_ಬಹುಲಕ) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ನಿಲ್ಲಿಸು;
		}
#ಆದರೆ_ಅಂತ್ಯ 	/* __GNU__ */
	}

leave:
	fclose(f);
	ಹಿಂತಿರುಗಿ ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
errcode_t ext2fs_check_mount_ಬಿಂದು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *device, ಪೂರ್ಣಾಂಕ *mount_flags,
				  ಅಕ್ಷರ *mtpt, ಪೂರ್ಣಾಂಕ mtlen)
{
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಂಕ 		busy = 0;

	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ಹಿಂತಿರುಗಿ 0;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__ /* This only works on Linux 2.6+ systems */
	{
		ರಚನೆ stat st_buf;

		ಆದರೆ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_ಬಹುಲಕ)) {
			ಪೂರ್ಣಾಂಕ fd = open(device, O_RDONLY | O_EXCL);

			ಆದರೆ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ಹಿಂತಿರುಗಿ 0;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (is_ಅದಲುಬದಲು_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_ಅದಲುಬದಲು;
		ಆದರೆ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ಇಲ್ಲದಿದ್ದರೆ {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#ಆದರೆ_ಅಂತ್ಯ
		*mount_flags = 0;
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_GETMNTINFO */
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_SETMNTENT */
	}
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *mount_flags)
{
	ಹಿಂತಿರುಗಿ ext2fs_check_mount_ಬಿಂದು(file, mount_flags, ಖಾಲಿ, 0);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ	retval, mount_flags;
	ಅಕ್ಷರ	mntpt[80];

	ಆದರೆ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}

	ಕೂಡಿಸು_error_ಕೋಷ್ಟಕ(&et_ext2_error_ಕೋಷ್ಟಕ);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_ಬಿಂದು(argv[1], &mount_flags,
					  mntpt, ಗಾತ್ರ(mntpt));
	ಆದರೆ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("Device %s reports flags %02x\n", argv[1], mount_flags);
	ಆದರೆ (mount_flags & EXT2_MF_BUSY)
		ಬರೆ("\t%s is apparently in use.\n", argv[1]);
	ಆದರೆ (mount_flags & EXT2_MF_MOUNTED)
		ಬರೆ("\t%s is mounted.\n", argv[1]);
	ಆದರೆ (mount_flags & EXT2_MF_ಅದಲುಬದಲು)
		ಬರೆ("\t%s is a swap device.\n", argv[1]);
	ಆದರೆ (mount_flags & EXT2_MF_READONLY)
		ಬರೆ("\t%s is read-only.\n", argv[1]);
	ಆದರೆ (mount_flags & EXT2_MF_ISROOT)
		ಬರೆ("\t%s is the root filesystem.\n", argv[1]);
	ಆದರೆ (mntpt[0])
		ಬರೆ("\t%s is mounted on %s.\n", argv[1], mntpt);
	ನಿರ್ಗಮಿಸು(0);
}
#ಆದರೆ_ಅಂತ್ಯ /* DEBUG */
