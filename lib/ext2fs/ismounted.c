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
#اگر_غیر_تعریف_شدہ BSD_ماخذ
#تعریف_کرو BSD_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ _DEFAULT_SOURCE
#تعریف_کرو _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#اگر_تعریف_شدہ HAVE_LINUX_FD_H
#شامل <linux/fd.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_LINUX_LOOP_H
#شامل <linux/loop.h>
#شامل <sys/ioctl.h>
#اگر_تعریف_شدہ HAVE_LINUX_MAJOR_H
#شامل <linux/major.h>
#اگر_ختم /* HAVE_LINUX_MAJOR_H */
#اگر_ختم /* HAVE_LINUX_LOOP_H */
#اگر_تعریف_شدہ HAVE_MNTENT_H
#شامل <mntent.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_GETMNTINFO
#شامل <paths.h>
#شامل <sys/param.h>
#شامل <sys/mount.h>
#اگر_ختم /* HAVE_GETMNTINFO */
#شامل <string.h>
#شامل <sys/stat.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_SYSMACROS_H
#شامل <sys/sysmacros.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#اگر_تعریف_شدہ HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ساکن صحیح check_loop_mounted(ثابت حرف *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#اگر defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ساخت loop_info64 loopinfo = {0, };
	صحیح loop_fd, ret;

	اگر (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		اگر (loop_fd < 0)
			واپس -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		اگر (ret < 0)
			واپس -1;

		اگر (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			واپس 1;
	}
#اگر_ختم /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	واپس 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ساکن errcode_t check_mntent_file(ثابت حرف *mtab_file, ثابت حرف *file,
				   صحیح *mount_flags, حرف *mtpt, صحیح mtlen)
{
	ساخت mntent 	*mnt;
	ساخت stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	صحیح		fd;

	*mount_flags = 0;

	اگر ((f = setmntent (mtab_file, "r")) == تہی) {
		اگر (errno == ENOENT) {
			اگر (ext2fs_محفوظ_getenv("EXT2FS_NO_MTAB_OK"))
				واپس 0;
			ورنہ
				واپس EXT2_ET_NO_MTAB_FILE;
		}
		واپس errno;
	}
	اگر (stat(file, &st_buf) == 0) {
		اگر (ext2fsP_is_disk_device(st_buf.st_منوال)) {
#اگر_غیر_تعریف_شدہ __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#اگر_ختم	/* __GNU__ */
		} ورنہ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	جب ((mnt = getmntent (f)) != تہی) {
		اگر (mnt->mnt_fsname[0] != '/')
			جاری;
		اگر (strcmp(file, mnt->mnt_fsname) == 0) {
			اگر (stat(mnt->mnt_dir, &st_buf) != 0)
				جاری;
			اگر (file_rdev && (file_rdev != st_buf.st_dev)) {
#اگر_تعریف_شدہ DEBUG
				لکھو("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#اگر_ختم /* DEBUG */
				جاری;
			}
			توڑو;
		}
		اگر (stat(mnt->mnt_fsname, &st_buf) == 0) {
			اگر (ext2fsP_is_disk_device(st_buf.st_منوال)) {
#اگر_غیر_تعریف_شدہ __GNU__
				اگر (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					اگر (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						جاری;
					اگر (file_rdev == dir_st_buf.st_dev)
						توڑو;
				}
				اگر (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					توڑو;
#اگر_ختم	/* __GNU__ */
			} ورنہ {
				اگر (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					توڑو;
			}
		}
	}

	اگر (mnt == 0) {
#اگر_غیر_تعریف_شدہ __GNU__ /* The GNU hurd is broken with respect to stat devices */
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
				جاؤ is_جڑ;
			}
		}
#اگر_ختم	/* __GNU__ */
		جاؤ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#اگر_تعریف_شدہ MNTOPT_RO
	/* Check to see if the ro option is set */
	اگر (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#اگر_ختم

	اگر (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	اگر (!strcmp(mnt->mnt_dir, "/")) {
is_جڑ:
#تعریف_کرو TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		اگر (fd < 0) {
			اگر (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ورنہ
			close(fd);
		(خالی) unlink(TEST_FILE);
	}

	اگر (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	واپس retval;
}

ساکن errcode_t check_mntent(ثابت حرف *file, صحیح *mount_flags,
			      حرف *mtpt, صحیح mtlen)
{
	errcode_t	retval;

#اگر_تعریف_شدہ DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	اگر (retval == 0)
		واپس 0;
#اگر_ختم /* DEBUG */
#اگر_تعریف_شدہ __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	اگر (retval == 0)
		واپس 0;
#اگر_ختم /* __linux__ */
#اگر defined(MOUNTED) || defined(_PATH_MOUNTED)
#اگر_غیر_تعریف_شدہ MOUNTED
#تعریف_کرو MOUNTED _PATH_MOUNTED
#اگر_ختم /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	واپس retval;
#ورنہ
	*mount_flags = 0;
	واپس 0;
#اگر_ختم /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ورنہ
#اگر defined(HAVE_GETMNTINFO)

ساکن errcode_t check_getmntinfo(ثابت حرف *file, صحیح *mount_flags,
				  حرف *mtpt, صحیح mtlen)
{
	ساخت statfs *mp;
        صحیح    len, n;
        ثابت  حرف   *s1;
	حرف	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        اگر (n == 0)
		واپس errno;

        len = حجم(_PATH_DEV) - 1;
        s1 = file;
        اگر (لڑی_موازنہ(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        جب (--n >= 0) {
                s2 = mp->f_mntfromname;
                اگر (لڑی_موازنہ(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                اگر (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			توڑو;
		}
                ++mp;
	}
	اگر (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	واپس 0;
}
#اگر_ختم /* HAVE_GETMNTINFO */
#اگر_ختم /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ساکن صحیح is_تبادلہ_device(ثابت حرف *file)
{
	FILE		*f;
	حرف		buf[1024], *cp;
	dev_t		file_dev;
	ساخت stat	st_buf;
	صحیح		ret = 0;

	file_dev = 0;
#اگر_غیر_تعریف_شدہ __GNU__ /* The GNU hurd is broken with respect to stat devices */
	اگر ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_منوال))
		file_dev = st_buf.st_rdev;
#اگر_ختم	/* __GNU__ */

	اگر (!(f = fopen("/proc/swaps", "r")))
		واپس 0;
	/* Skip the first line */
	اگر (!fgets(buf, حجم(buf), f))
		جاؤ leave;
	اگر (*buf && لڑی_موازنہ(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		جاؤ valid_پہلا_سطر;

	جب (fgets(buf, حجم(buf), f)) {
valid_پہلا_سطر:
		اگر ((cp = strchr(buf, ' ')) != تہی)
			*cp = 0;
		اگر ((cp = strchr(buf, '\t')) != تہی)
			*cp = 0;
		اگر (strcmp(buf, file) == 0) {
			ret++;
			توڑو;
		}
#اگر_غیر_تعریف_شدہ __GNU__
		اگر (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_منوال) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			توڑو;
		}
#اگر_ختم 	/* __GNU__ */
	}

leave:
	fclose(f);
	واپس ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
errcode_t ext2fs_check_mount_نقطہ(ثابت حرف *device, صحیح *mount_flags,
				  حرف *mtpt, صحیح mtlen)
{
	errcode_t	retval = 0;
	صحیح 		busy = 0;

	اگر (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		اگر (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		واپس 0;
	}
	اگر (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		اگر (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		واپس 0;
	}

#اگر_تعریف_شدہ __linux__ /* This only works on Linux 2.6+ systems */
	{
		ساخت stat st_buf;

		اگر (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_منوال)) {
			صحیح fd = open(device, O_RDONLY | O_EXCL);

			اگر (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				واپس 0;
			} ورنہ اگر (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#اگر_ختم

	اگر (is_تبادلہ_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_تبادلہ;
		اگر (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ورنہ {
#اگر_تعریف_شدہ HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ورنہ
#اگر_تعریف_شدہ HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ورنہ
#اگر defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#اگر_ختم
		*mount_flags = 0;
#اگر_ختم /* HAVE_GETMNTINFO */
#اگر_ختم /* HAVE_SETMNTENT */
	}
	اگر (retval)
		واپس retval;

	اگر (busy)
		*mount_flags |= EXT2_MF_BUSY;

	واپس 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ثابت حرف *file, صحیح *mount_flags)
{
	واپس ext2fs_check_mount_نقطہ(file, mount_flags, تہی, 0);
}

#اگر_تعریف_شدہ DEBUG
صحیح مین(صحیح argc, حرف **argv)
{
	صحیح	retval, mount_flags;
	حرف	mntpt[80];

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		باہر_نکلو(1);
	}

	جوڑو_error_جدول(&et_ext2_error_جدول);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_نقطہ(argv[1], &mount_flags,
					  mntpt, حجم(mntpt));
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		باہر_نکلو(1);
	}
	لکھو("Device %s reports flags %02x\n", argv[1], mount_flags);
	اگر (mount_flags & EXT2_MF_BUSY)
		لکھو("\t%s is apparently in use.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_MOUNTED)
		لکھو("\t%s is mounted.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_تبادلہ)
		لکھو("\t%s is a swap device.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_READONLY)
		لکھو("\t%s is read-only.\n", argv[1]);
	اگر (mount_flags & EXT2_MF_ISROOT)
		لکھو("\t%s is the root filesystem.\n", argv[1]);
	اگر (mntpt[0])
		لکھو("\t%s is mounted on %s.\n", argv[1], mntpt);
	باہر_نکلو(0);
}
#اگر_ختم /* DEBUG */
