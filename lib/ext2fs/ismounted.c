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
#جے_بنا_تعریف BSD_سروت
#تعریف_کرو BSD_سروت
#جے_انت
#جے_بنا_تعریف _DEFAULT_SOURCE
#تعریف_کرو _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#جے_تعریف_شدہ HAVE_LINUX_FD_H
#ضمن <linux/fd.h>
#جے_انت
#جے_تعریف_شدہ HAVE_LINUX_LOOP_H
#ضمن <linux/loop.h>
#ضمن <sys/ioctl.h>
#جے_تعریف_شدہ HAVE_LINUX_MAJOR_H
#ضمن <linux/major.h>
#جے_انت /* HAVE_LINUX_MAJOR_H */
#جے_انت /* HAVE_LINUX_LOOP_H */
#جے_تعریف_شدہ HAVE_MNTENT_H
#ضمن <mntent.h>
#جے_انت
#جے_تعریف_شدہ HAVE_GETMNTINFO
#ضمن <paths.h>
#ضمن <sys/param.h>
#ضمن <sys/mount.h>
#جے_انت /* HAVE_GETMNTINFO */
#ضمن <string.h>
#ضمن <sys/stat.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#جے_تعریف_شدہ HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ساكن صحيح check_loop_mounted(ثابت حرف *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#إذا defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	بنية loop_info64 loopinfo = {0, };
	صحيح loop_fd, ret;

	إذا (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		إذا (loop_fd < 0)
			أرجع -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		إذا (ret < 0)
			أرجع -1;

		إذا (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			أرجع 1;
	}
#جے_انت /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	أرجع 0;
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

	إذا ((f = setmntent (mtab_file, "r")) == عدم) {
		إذا (errno == ENOENT) {
			إذا (ext2fs_محفوظ_getenv("EXT2FS_NO_MTAB_OK"))
				أرجع 0;
			وإلا
				أرجع EXT2_ET_NO_MTAB_FILE;
		}
		أرجع errno;
	}
	إذا (stat(file, &st_buf) == 0) {
		إذا (ext2fsP_is_disk_device(st_buf.st_منوال)) {
#جے_بنا_تعریف __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#جے_انت	/* __GNU__ */
		} وإلا {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	طالما ((mnt = getmntent (f)) != عدم) {
		إذا (mnt->mnt_fsname[0] != '/')
			استمر;
		إذا (strcmp(file, mnt->mnt_fsname) == 0) {
			إذا (stat(mnt->mnt_dir, &st_buf) != 0)
				استمر;
			إذا (file_rdev && (file_rdev != st_buf.st_dev)) {
#جے_تعریف_شدہ DEBUG
				لکھو("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#جے_انت /* DEBUG */
				استمر;
			}
			اكسر;
		}
		إذا (stat(mnt->mnt_fsname, &st_buf) == 0) {
			إذا (ext2fsP_is_disk_device(st_buf.st_منوال)) {
#جے_بنا_تعریف __GNU__
				إذا (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					إذا (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						استمر;
					إذا (file_rdev == dir_st_buf.st_dev)
						اكسر;
				}
				إذا (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					اكسر;
#جے_انت	/* __GNU__ */
			} وإلا {
				إذا (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					اكسر;
			}
		}
	}

	إذا (mnt == 0) {
#جے_بنا_تعریف __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		إذا (file_rdev && stat("/", &st_buf) == 0) {
			إذا (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				إذا (mtpt)
					strncpy(mtpt, "/", mtlen);
				اذهب is_جڑ;
			}
		}
#جے_انت	/* __GNU__ */
		اذهب errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#جے_تعریف_شدہ MNTOPT_RO
	/* Check to see if the ro option is set */
	إذا (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#جے_انت

	إذا (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	إذا (!strcmp(mnt->mnt_dir, "/")) {
is_جڑ:
#تعریف_کرو TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		إذا (fd < 0) {
			إذا (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} وإلا
			close(fd);
		(فراغ) unlink(TEST_FILE);
	}

	إذا (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	أرجع retval;
}

ساكن errcode_t check_mntent(ثابت حرف *file, صحيح *mount_flags,
			      حرف *mtpt, صحيح mtlen)
{
	errcode_t	retval;

#جے_تعریف_شدہ DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	إذا (retval == 0)
		أرجع 0;
#جے_انت /* DEBUG */
#جے_تعریف_شدہ __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	إذا (retval == 0)
		أرجع 0;
#جے_انت /* __linux__ */
#إذا defined(MOUNTED) || defined(_PATH_MOUNTED)
#جے_بنا_تعریف MOUNTED
#تعریف_کرو MOUNTED _PATH_MOUNTED
#جے_انت /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	أرجع retval;
#وإلا
	*mount_flags = 0;
	أرجع 0;
#جے_انت /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#وإلا
#إذا defined(HAVE_GETMNTINFO)

ساكن errcode_t check_getmntinfo(ثابت حرف *file, صحيح *mount_flags,
				  حرف *mtpt, صحيح mtlen)
{
	بنية statfs *mp;
        صحيح    len, n;
        ثابت  حرف   *s1;
	حرف	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        إذا (n == 0)
		أرجع errno;

        len = حجم(_PATH_DEV) - 1;
        s1 = file;
        إذا (لڑی_مقابلہ(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        طالما (--n >= 0) {
                s2 = mp->f_mntfromname;
                إذا (لڑی_مقابلہ(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                إذا (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			اكسر;
		}
                ++mp;
	}
	إذا (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	أرجع 0;
}
#جے_انت /* HAVE_GETMNTINFO */
#جے_انت /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ساكن صحيح is_وٹاندرا_device(ثابت حرف *file)
{
	FILE		*f;
	حرف		buf[1024], *cp;
	dev_t		file_dev;
	بنية stat	st_buf;
	صحيح		ret = 0;

	file_dev = 0;
#جے_بنا_تعریف __GNU__ /* The GNU hurd is broken with respect to stat devices */
	إذا ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_منوال))
		file_dev = st_buf.st_rdev;
#جے_انت	/* __GNU__ */

	إذا (!(f = fopen("/proc/swaps", "r")))
		أرجع 0;
	/* Skip the first line */
	إذا (!fgets(buf, حجم(buf), f))
		اذهب leave;
	إذا (*buf && لڑی_مقابلہ(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		اذهب valid_پہلا_سطر;

	طالما (fgets(buf, حجم(buf), f)) {
valid_پہلا_سطر:
		إذا ((cp = strchr(buf, ' ')) != عدم)
			*cp = 0;
		إذا ((cp = strchr(buf, '\t')) != عدم)
			*cp = 0;
		إذا (strcmp(buf, file) == 0) {
			ret++;
			اكسر;
		}
#جے_بنا_تعریف __GNU__
		إذا (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_منوال) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			اكسر;
		}
#جے_انت 	/* __GNU__ */
	}

leave:
	fclose(f);
	أرجع ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#جے_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#جے_انت
errcode_t ext2fs_check_mount_نقطہ(ثابت حرف *device, صحيح *mount_flags,
				  حرف *mtpt, صحيح mtlen)
{
	errcode_t	retval = 0;
	صحيح 		busy = 0;

	إذا (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		إذا (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		أرجع 0;
	}
	إذا (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		إذا (ext2fs_محفوظ_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		أرجع 0;
	}

#جے_تعریف_شدہ __linux__ /* This only works on Linux 2.6+ systems */
	{
		بنية stat st_buf;

		إذا (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_منوال)) {
			صحيح fd = open(device, O_RDONLY | O_EXCL);

			إذا (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				أرجع 0;
			} وإلا إذا (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#جے_انت

	إذا (is_وٹاندرا_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_وٹاندرا;
		إذا (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} وإلا {
#جے_تعریف_شدہ HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#وإلا
#جے_تعریف_شدہ HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#وإلا
#إذا defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#جے_انت
		*mount_flags = 0;
#جے_انت /* HAVE_GETMNTINFO */
#جے_انت /* HAVE_SETMNTENT */
	}
	إذا (retval)
		أرجع retval;

	إذا (busy)
		*mount_flags |= EXT2_MF_BUSY;

	أرجع 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ثابت حرف *file, صحيح *mount_flags)
{
	أرجع ext2fs_check_mount_نقطہ(file, mount_flags, عدم, 0);
}

#جے_تعریف_شدہ DEBUG
صحيح مکھ(صحيح argc, حرف **argv)
{
	صحيح	retval, mount_flags;
	حرف	mntpt[80];

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		باہر_نکلو(1);
	}

	جوڑو_error_جدول(&et_ext2_error_جدول);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_نقطہ(argv[1], &mount_flags,
					  mntpt, حجم(mntpt));
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		باہر_نکلو(1);
	}
	لکھو("Device %s reports flags %02x\n", argv[1], mount_flags);
	إذا (mount_flags & EXT2_MF_BUSY)
		لکھو("\t%s is apparently in use.\n", argv[1]);
	إذا (mount_flags & EXT2_MF_MOUNTED)
		لکھو("\t%s is mounted.\n", argv[1]);
	إذا (mount_flags & EXT2_MF_وٹاندرا)
		لکھو("\t%s is a swap device.\n", argv[1]);
	إذا (mount_flags & EXT2_MF_READONLY)
		لکھو("\t%s is read-only.\n", argv[1]);
	إذا (mount_flags & EXT2_MF_ISROOT)
		لکھو("\t%s is the root filesystem.\n", argv[1]);
	إذا (mntpt[0])
		لکھو("\t%s is mounted on %s.\n", argv[1], mntpt);
	باہر_نکلو(0);
}
#جے_انت /* DEBUG */
