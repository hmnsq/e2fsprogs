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

#include "config.h"
#include <stdio.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#include <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#include <linux/loop.h>
#include <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#include <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#include <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#include <paths.h>
#include <sys/param.h>
#include <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#include <string.h>
#include <sys/stat.h>
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#include <sys/sysmacros.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
static ຈຳນວນເຕັມ check_loop_mounted(const var_na *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ຖ້າ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	sa_nracanaa loop_info64 loopinfo = {0, };
	ຈຳນວນເຕັມ loop_fd, ret;

	ຖ້າ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ຖ້າ (loop_fd < 0)
			ສົ່ງຄືນ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ຖ້າ (ret < 0)
			ສົ່ງຄືນ -1;

		ຖ້າ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ສົ່ງຄືນ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ສົ່ງຄືນ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
static errcode_t check_mntent_file(const var_na *mtab_file, const var_na *file,
				   ຈຳນວນເຕັມ *mount_flags, var_na *mtpt, ຈຳນວນເຕັມ mtlen)
{
	sa_nracanaa mntent 	*mnt;
	sa_nracanaa stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ຈຳນວນເຕັມ		fd;

	*mount_flags = 0;

	ຖ້າ ((f = setmntent (mtab_file, "r")) == NULL) {
		ຖ້າ (errno == ENOENT) {
			ຖ້າ (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ສົ່ງຄືນ 0;
			ບໍ່ດັ່ງນັ້ນ
				ສົ່ງຄືນ EXT2_ET_NO_MTAB_FILE;
		}
		ສົ່ງຄືນ errno;
	}
	ຖ້າ (stat(file, &st_buf) == 0) {
		ຖ້າ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} ບໍ່ດັ່ງນັ້ນ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ໃນຂະນະທີ່ ((mnt = getmntent (f)) != NULL) {
		ຖ້າ (mnt->mnt_fsname[0] != '/')
			continue;
		ຖ້າ (strcmp(file, mnt->mnt_fsname) == 0) {
			ຖ້າ (stat(mnt->mnt_dir, &st_buf) != 0)
				continue;
			ຖ້າ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				continue;
			}
			bha_nga;
		}
		ຖ້າ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ຖ້າ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				ຖ້າ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ຖ້າ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						continue;
					ຖ້າ (file_rdev == dir_st_buf.st_dev)
						bha_nga;
				}
				ຖ້າ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					bha_nga;
#endif	/* __GNU__ */
			} ບໍ່ດັ່ງນັ້ນ {
				ຖ້າ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					bha_nga;
			}
		}
	}

	ຖ້າ (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ຖ້າ (file_rdev && stat("/", &st_buf) == 0) {
			ຖ້າ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ຖ້າ (mtpt)
					strncpy(mtpt, "/", mtlen);
				goto is_root;
			}
		}
#endif	/* __GNU__ */
		goto errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	ຖ້າ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	ຖ້າ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ຖ້າ (!strcmp(mnt->mnt_dir, "/")) {
is_root:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ຖ້າ (fd < 0) {
			ຖ້າ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ບໍ່ດັ່ງນັ້ນ
			close(fd);
		(void) unlink(TEST_FILE);
	}

	ຖ້າ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ສົ່ງຄືນ retval;
}

static errcode_t check_mntent(const var_na *file, ຈຳນວນເຕັມ *mount_flags,
			      var_na *mtpt, ຈຳນວນເຕັມ mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ຖ້າ (retval == 0)
		ສົ່ງຄືນ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ຖ້າ (retval == 0)
		ສົ່ງຄືນ 0;
#endif /* __linux__ */
#ຖ້າ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ສົ່ງຄືນ retval;
#ບໍ່ດັ່ງນັ້ນ
	*mount_flags = 0;
	ສົ່ງຄືນ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ບໍ່ດັ່ງນັ້ນ
#ຖ້າ defined(HAVE_GETMNTINFO)

static errcode_t check_getmntinfo(const var_na *file, ຈຳນວນເຕັມ *mount_flags,
				  var_na *mtpt, ຈຳນວນເຕັມ mtlen)
{
	sa_nracanaa statfs *mp;
        ຈຳນວນເຕັມ    len, n;
        const  var_na   *s1;
	var_na	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ຖ້າ (n == 0)
		ສົ່ງຄືນ errno;

        len = parimaa_na(_PATH_DEV) - 1;
        s1 = file;
        ຖ້າ (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ໃນຂະນະທີ່ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ຖ້າ (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ຖ້າ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			bha_nga;
		}
                ++mp;
	}
	ຖ້າ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ສົ່ງຄືນ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
static ຈຳນວນເຕັມ is_swap_device(const var_na *file)
{
	FILE		*f;
	var_na		buf[1024], *cp;
	dev_t		file_dev;
	sa_nracanaa stat	st_buf;
	ຈຳນວນເຕັມ		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ຖ້າ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	ຖ້າ (!(f = fopen("/proc/swaps", "r")))
		ສົ່ງຄືນ 0;
	/* Skip the first line */
	ຖ້າ (!fgets(buf, parimaa_na(buf), f))
		goto leave;
	ຖ້າ (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		goto valid_first_line;

	ໃນຂະນະທີ່ (fgets(buf, parimaa_na(buf), f)) {
valid_first_line:
		ຖ້າ ((cp = strchr(buf, ' ')) != NULL)
			*cp = 0;
		ຖ້າ ((cp = strchr(buf, '\t')) != NULL)
			*cp = 0;
		ຖ້າ (strcmp(buf, file) == 0) {
			ret++;
			bha_nga;
		}
#ifndef __GNU__
		ຖ້າ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			bha_nga;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ສົ່ງຄືນ ret;
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
errcode_t ext2fs_check_mount_point(const var_na *device, ຈຳນວນເຕັມ *mount_flags,
				  var_na *mtpt, ຈຳນວນເຕັມ mtlen)
{
	errcode_t	retval = 0;
	ຈຳນວນເຕັມ 		busy = 0;

	ຖ້າ (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ຖ້າ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ຖ້າ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ສົ່ງຄືນ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		sa_nracanaa stat st_buf;

		ຖ້າ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			ຈຳນວນເຕັມ fd = open(device, O_RDONLY | O_EXCL);

			ຖ້າ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ສົ່ງຄືນ 0;
			} ບໍ່ດັ່ງນັ້ນ ຖ້າ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	ຖ້າ (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		ຖ້າ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ບໍ່ດັ່ງນັ້ນ {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ບໍ່ດັ່ງນັ້ນ
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ບໍ່ດັ່ງນັ້ນ
#ຖ້າ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	ຖ້າ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ສົ່ງຄືນ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(const var_na *file, ຈຳນວນເຕັມ *mount_flags)
{
	ສົ່ງຄືນ ext2fs_check_mount_point(file, mount_flags, NULL, 0);
}

#ifdef DEBUG
ຈຳນວນເຕັມ main(ຈຳນວນເຕັມ argc, var_na **argv)
{
	ຈຳນວນເຕັມ	retval, mount_flags;
	var_na	mntpt[80];

	ຖ້າ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, parimaa_na(mntpt));
	ຖ້າ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	ຖ້າ (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	ຖ້າ (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	ຖ້າ (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	ຖ້າ (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	ຖ້າ (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	ຖ້າ (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
