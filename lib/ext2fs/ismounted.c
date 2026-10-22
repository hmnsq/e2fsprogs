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
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#如果 HAVE_ERRNO_H
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
#如果 HAVE_SYS_TYPES_H
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
static 整数 check_loop_mounted(const char *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#如果 defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	struct loop_info64 loopinfo = {0, };
	整数 loop_fd, ret;

	如果 (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		如果 (loop_fd < 0)
			返回 -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		如果 (ret < 0)
			返回 -1;

		如果 (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			返回 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	返回 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
static errcode_t check_mntent_file(const char *mtab_file, const char *file,
				   整数 *mount_flags, char *mtpt, 整数 mtlen)
{
	struct mntent 	*mnt;
	struct stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	整数		fd;

	*mount_flags = 0;

	如果 ((f = setmntent (mtab_file, "r")) == NULL) {
		如果 (errno == ENOENT) {
			如果 (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				返回 0;
			否则
				返回 EXT2_ET_NO_MTAB_FILE;
		}
		返回 errno;
	}
	如果 (stat(file, &st_buf) == 0) {
		如果 (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} 否则 {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	当 ((mnt = getmntent (f)) != NULL) {
		如果 (mnt->mnt_fsname[0] != '/')
			continue;
		如果 (strcmp(file, mnt->mnt_fsname) == 0) {
			如果 (stat(mnt->mnt_dir, &st_buf) != 0)
				continue;
			如果 (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				continue;
			}
			break;
		}
		如果 (stat(mnt->mnt_fsname, &st_buf) == 0) {
			如果 (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				如果 (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					如果 (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						continue;
					如果 (file_rdev == dir_st_buf.st_dev)
						break;
				}
				如果 (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					break;
#endif	/* __GNU__ */
			} 否则 {
				如果 (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					break;
			}
		}
	}

	如果 (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		如果 (file_rdev && stat("/", &st_buf) == 0) {
			如果 (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				如果 (mtpt)
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
	如果 (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	如果 (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	如果 (!strcmp(mnt->mnt_dir, "/")) {
is_root:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		如果 (fd < 0) {
			如果 (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} 否则
			close(fd);
		(void) unlink(TEST_FILE);
	}

	如果 (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	返回 retval;
}

static errcode_t check_mntent(const char *file, 整数 *mount_flags,
			      char *mtpt, 整数 mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	如果 (retval == 0)
		返回 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	如果 (retval == 0)
		返回 0;
#endif /* __linux__ */
#如果 defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	返回 retval;
#否则
	*mount_flags = 0;
	返回 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#否则
#如果 defined(HAVE_GETMNTINFO)

static errcode_t check_getmntinfo(const char *file, 整数 *mount_flags,
				  char *mtpt, 整数 mtlen)
{
	struct statfs *mp;
        整数    len, n;
        const  char   *s1;
	char	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        如果 (n == 0)
		返回 errno;

        len = sizeof(_PATH_DEV) - 1;
        s1 = file;
        如果 (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        当 (--n >= 0) {
                s2 = mp->f_mntfromname;
                如果 (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                如果 (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			break;
		}
                ++mp;
	}
	如果 (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	返回 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
static 整数 is_swap_device(const char *file)
{
	FILE		*f;
	char		buf[1024], *cp;
	dev_t		file_dev;
	struct stat	st_buf;
	整数		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	如果 ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	如果 (!(f = fopen("/proc/swaps", "r")))
		返回 0;
	/* Skip the first line */
	如果 (!fgets(buf, sizeof(buf), f))
		goto leave;
	如果 (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		goto valid_first_line;

	当 (fgets(buf, sizeof(buf), f)) {
valid_first_line:
		如果 ((cp = strchr(buf, ' ')) != NULL)
			*cp = 0;
		如果 ((cp = strchr(buf, '\t')) != NULL)
			*cp = 0;
		如果 (strcmp(buf, file) == 0) {
			ret++;
			break;
		}
#ifndef __GNU__
		如果 (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			break;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	返回 ret;
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
errcode_t ext2fs_check_mount_point(const char *device, 整数 *mount_flags,
				  char *mtpt, 整数 mtlen)
{
	errcode_t	retval = 0;
	整数 		busy = 0;

	如果 (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		如果 (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		返回 0;
	}
	如果 (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		如果 (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		返回 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		struct stat st_buf;

		如果 (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			整数 fd = open(device, O_RDONLY | O_EXCL);

			如果 (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				返回 0;
			} 否则 如果 (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	如果 (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		如果 (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} 否则 {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#否则
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#否则
#如果 defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	如果 (retval)
		返回 retval;

	如果 (busy)
		*mount_flags |= EXT2_MF_BUSY;

	返回 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(const char *file, 整数 *mount_flags)
{
	返回 ext2fs_check_mount_point(file, mount_flags, NULL, 0);
}

#ifdef DEBUG
整数 主要(整数 argc, char **argv)
{
	整数	retval, mount_flags;
	char	mntpt[80];

	如果 (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, sizeof(mntpt));
	如果 (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	如果 (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	如果 (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	如果 (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	如果 (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	如果 (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	如果 (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
