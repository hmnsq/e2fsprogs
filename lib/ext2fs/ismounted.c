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
#ᮚᮦᮔ᮪ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ᮚᮦᮔ᮪ HAVE_ERRNO_H
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
#ᮚᮦᮔ᮪ HAVE_SYS_TYPES_H
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
static puur_naa_nka check_loop_mounted(const var_na *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ᮚᮦᮔ᮪ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	sa_nracanaa loop_info64 loopinfo = {0, };
	puur_naa_nka loop_fd, ret;

	ᮚᮦᮔ᮪ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ᮚᮦᮔ᮪ (loop_fd < 0)
			ᮝᮜᮤ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ᮚᮦᮔ᮪ (ret < 0)
			ᮝᮜᮤ -1;

		ᮚᮦᮔ᮪ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ᮝᮜᮤ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ᮝᮜᮤ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
static errcode_t check_mntent_file(const var_na *mtab_file, const var_na *file,
				   puur_naa_nka *mount_flags, var_na *mtpt, puur_naa_nka mtlen)
{
	sa_nracanaa mntent 	*mnt;
	sa_nracanaa stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	puur_naa_nka		fd;

	*mount_flags = 0;

	ᮚᮦᮔ᮪ ((f = setmntent (mtab_file, "r")) == NULL) {
		ᮚᮦᮔ᮪ (errno == ENOENT) {
			ᮚᮦᮔ᮪ (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ᮝᮜᮤ 0;
			ᮜᮤᮚ
				ᮝᮜᮤ EXT2_ET_NO_MTAB_FILE;
		}
		ᮝᮜᮤ errno;
	}
	ᮚᮦᮔ᮪ (stat(file, &st_buf) == 0) {
		ᮚᮦᮔ᮪ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} ᮜᮤᮚ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ᮞᮨᮓᮨᮜ ((mnt = getmntent (f)) != NULL) {
		ᮚᮦᮔ᮪ (mnt->mnt_fsname[0] != '/')
			continue;
		ᮚᮦᮔ᮪ (strcmp(file, mnt->mnt_fsname) == 0) {
			ᮚᮦᮔ᮪ (stat(mnt->mnt_dir, &st_buf) != 0)
				continue;
			ᮚᮦᮔ᮪ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				continue;
			}
			bha_nga;
		}
		ᮚᮦᮔ᮪ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ᮚᮦᮔ᮪ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				ᮚᮦᮔ᮪ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ᮚᮦᮔ᮪ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						continue;
					ᮚᮦᮔ᮪ (file_rdev == dir_st_buf.st_dev)
						bha_nga;
				}
				ᮚᮦᮔ᮪ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					bha_nga;
#endif	/* __GNU__ */
			} ᮜᮤᮚ {
				ᮚᮦᮔ᮪ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					bha_nga;
			}
		}
	}

	ᮚᮦᮔ᮪ (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ᮚᮦᮔ᮪ (file_rdev && stat("/", &st_buf) == 0) {
			ᮚᮦᮔ᮪ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ᮚᮦᮔ᮪ (mtpt)
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
	ᮚᮦᮔ᮪ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	ᮚᮦᮔ᮪ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ᮚᮦᮔ᮪ (!strcmp(mnt->mnt_dir, "/")) {
is_root:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ᮚᮦᮔ᮪ (fd < 0) {
			ᮚᮦᮔ᮪ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ᮜᮤᮚ
			close(fd);
		(void) unlink(TEST_FILE);
	}

	ᮚᮦᮔ᮪ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ᮝᮜᮤ retval;
}

static errcode_t check_mntent(const var_na *file, puur_naa_nka *mount_flags,
			      var_na *mtpt, puur_naa_nka mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ᮚᮦᮔ᮪ (retval == 0)
		ᮝᮜᮤ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ᮚᮦᮔ᮪ (retval == 0)
		ᮝᮜᮤ 0;
#endif /* __linux__ */
#ᮚᮦᮔ᮪ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ᮝᮜᮤ retval;
#ᮜᮤᮚ
	*mount_flags = 0;
	ᮝᮜᮤ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ᮜᮤᮚ
#ᮚᮦᮔ᮪ defined(HAVE_GETMNTINFO)

static errcode_t check_getmntinfo(const var_na *file, puur_naa_nka *mount_flags,
				  var_na *mtpt, puur_naa_nka mtlen)
{
	sa_nracanaa statfs *mp;
        puur_naa_nka    len, n;
        const  var_na   *s1;
	var_na	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ᮚᮦᮔ᮪ (n == 0)
		ᮝᮜᮤ errno;

        len = parimaa_na(_PATH_DEV) - 1;
        s1 = file;
        ᮚᮦᮔ᮪ (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ᮞᮨᮓᮨᮜ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ᮚᮦᮔ᮪ (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ᮚᮦᮔ᮪ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			bha_nga;
		}
                ++mp;
	}
	ᮚᮦᮔ᮪ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ᮝᮜᮤ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
static puur_naa_nka is_swap_device(const var_na *file)
{
	FILE		*f;
	var_na		buf[1024], *cp;
	dev_t		file_dev;
	sa_nracanaa stat	st_buf;
	puur_naa_nka		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ᮚᮦᮔ᮪ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	ᮚᮦᮔ᮪ (!(f = fopen("/proc/swaps", "r")))
		ᮝᮜᮤ 0;
	/* Skip the first line */
	ᮚᮦᮔ᮪ (!fgets(buf, parimaa_na(buf), f))
		goto leave;
	ᮚᮦᮔ᮪ (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		goto valid_first_line;

	ᮞᮨᮓᮨᮜ (fgets(buf, parimaa_na(buf), f)) {
valid_first_line:
		ᮚᮦᮔ᮪ ((cp = strchr(buf, ' ')) != NULL)
			*cp = 0;
		ᮚᮦᮔ᮪ ((cp = strchr(buf, '\t')) != NULL)
			*cp = 0;
		ᮚᮦᮔ᮪ (strcmp(buf, file) == 0) {
			ret++;
			bha_nga;
		}
#ifndef __GNU__
		ᮚᮦᮔ᮪ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			bha_nga;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ᮝᮜᮤ ret;
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
errcode_t ext2fs_check_mount_point(const var_na *device, puur_naa_nka *mount_flags,
				  var_na *mtpt, puur_naa_nka mtlen)
{
	errcode_t	retval = 0;
	puur_naa_nka 		busy = 0;

	ᮚᮦᮔ᮪ (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ᮚᮦᮔ᮪ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ᮝᮜᮤ 0;
	}
	ᮚᮦᮔ᮪ (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ᮚᮦᮔ᮪ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ᮝᮜᮤ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		sa_nracanaa stat st_buf;

		ᮚᮦᮔ᮪ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			puur_naa_nka fd = open(device, O_RDONLY | O_EXCL);

			ᮚᮦᮔ᮪ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ᮝᮜᮤ 0;
			} ᮜᮤᮚ ᮚᮦᮔ᮪ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	ᮚᮦᮔ᮪ (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		ᮚᮦᮔ᮪ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ᮜᮤᮚ {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ᮜᮤᮚ
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ᮜᮤᮚ
#ᮚᮦᮔ᮪ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	ᮚᮦᮔ᮪ (retval)
		ᮝᮜᮤ retval;

	ᮚᮦᮔ᮪ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ᮝᮜᮤ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(const var_na *file, puur_naa_nka *mount_flags)
{
	ᮝᮜᮤ ext2fs_check_mount_point(file, mount_flags, NULL, 0);
}

#ifdef DEBUG
puur_naa_nka main(puur_naa_nka argc, var_na **argv)
{
	puur_naa_nka	retval, mount_flags;
	var_na	mntpt[80];

	ᮚᮦᮔ᮪ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, parimaa_na(mntpt));
	ᮚᮦᮔ᮪ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	ᮚᮦᮔ᮪ (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	ᮚᮦᮔ᮪ (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	ᮚᮦᮔ᮪ (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	ᮚᮦᮔ᮪ (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	ᮚᮦᮔ᮪ (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	ᮚᮦᮔ᮪ (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
