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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#якщо HAVE_ERRNO_H
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
#якщо HAVE_SYS_TYPES_H
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
static ціле check_loop_mounted(const char *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#якщо defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	struct loop_info64 loopinfo = {0, };
	ціле loop_fd, ret;

	якщо (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		якщо (loop_fd < 0)
			повернути -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		якщо (ret < 0)
			повернути -1;

		якщо (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			повернути 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	повернути 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
static errcode_t check_mntent_file(const char *mtab_file, const char *file,
				   ціле *mount_flags, char *mtpt, ціле mtlen)
{
	struct mntent 	*mnt;
	struct stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ціле		fd;

	*mount_flags = 0;

	якщо ((f = setmntent (mtab_file, "r")) == NULL) {
		якщо (errno == ENOENT) {
			якщо (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				повернути 0;
			інакше
				повернути EXT2_ET_NO_MTAB_FILE;
		}
		повернути errno;
	}
	якщо (stat(file, &st_buf) == 0) {
		якщо (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} інакше {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	поки ((mnt = getmntent (f)) != NULL) {
		якщо (mnt->mnt_fsname[0] != '/')
			continue;
		якщо (strcmp(file, mnt->mnt_fsname) == 0) {
			якщо (stat(mnt->mnt_dir, &st_buf) != 0)
				continue;
			якщо (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				continue;
			}
			break;
		}
		якщо (stat(mnt->mnt_fsname, &st_buf) == 0) {
			якщо (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				якщо (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					якщо (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						continue;
					якщо (file_rdev == dir_st_buf.st_dev)
						break;
				}
				якщо (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					break;
#endif	/* __GNU__ */
			} інакше {
				якщо (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					break;
			}
		}
	}

	якщо (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		якщо (file_rdev && stat("/", &st_buf) == 0) {
			якщо (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				якщо (mtpt)
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
	якщо (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	якщо (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	якщо (!strcmp(mnt->mnt_dir, "/")) {
is_root:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		якщо (fd < 0) {
			якщо (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} інакше
			close(fd);
		(void) unlink(TEST_FILE);
	}

	якщо (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	повернути retval;
}

static errcode_t check_mntent(const char *file, ціле *mount_flags,
			      char *mtpt, ціле mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	якщо (retval == 0)
		повернути 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	якщо (retval == 0)
		повернути 0;
#endif /* __linux__ */
#якщо defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	повернути retval;
#інакше
	*mount_flags = 0;
	повернути 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#інакше
#якщо defined(HAVE_GETMNTINFO)

static errcode_t check_getmntinfo(const char *file, ціле *mount_flags,
				  char *mtpt, ціле mtlen)
{
	struct statfs *mp;
        ціле    len, n;
        const  char   *s1;
	char	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        якщо (n == 0)
		повернути errno;

        len = sizeof(_PATH_DEV) - 1;
        s1 = file;
        якщо (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        поки (--n >= 0) {
                s2 = mp->f_mntfromname;
                якщо (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                якщо (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			break;
		}
                ++mp;
	}
	якщо (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	повернути 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
static ціле is_swap_device(const char *file)
{
	FILE		*f;
	char		buf[1024], *cp;
	dev_t		file_dev;
	struct stat	st_buf;
	ціле		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	якщо ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	якщо (!(f = fopen("/proc/swaps", "r")))
		повернути 0;
	/* Skip the first line */
	якщо (!fgets(buf, sizeof(buf), f))
		goto leave;
	якщо (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		goto valid_first_line;

	поки (fgets(buf, sizeof(buf), f)) {
valid_first_line:
		якщо ((cp = strchr(buf, ' ')) != NULL)
			*cp = 0;
		якщо ((cp = strchr(buf, '\t')) != NULL)
			*cp = 0;
		якщо (strcmp(buf, file) == 0) {
			ret++;
			break;
		}
#ifndef __GNU__
		якщо (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			break;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	повернути ret;
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
errcode_t ext2fs_check_mount_point(const char *device, ціле *mount_flags,
				  char *mtpt, ціле mtlen)
{
	errcode_t	retval = 0;
	ціле 		busy = 0;

	якщо (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		якщо (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		повернути 0;
	}
	якщо (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		якщо (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		повернути 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		struct stat st_buf;

		якщо (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			ціле fd = open(device, O_RDONLY | O_EXCL);

			якщо (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				повернути 0;
			} інакше якщо (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	якщо (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		якщо (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} інакше {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#інакше
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#інакше
#якщо defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	якщо (retval)
		повернути retval;

	якщо (busy)
		*mount_flags |= EXT2_MF_BUSY;

	повернути 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(const char *file, ціле *mount_flags)
{
	повернути ext2fs_check_mount_point(file, mount_flags, NULL, 0);
}

#ifdef DEBUG
ціле main(ціле argc, char **argv)
{
	ціле	retval, mount_flags;
	char	mntpt[80];

	якщо (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, sizeof(mntpt));
	якщо (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	якщо (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	якщо (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	якщо (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	якщо (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	якщо (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	якщо (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
