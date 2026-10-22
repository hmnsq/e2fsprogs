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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#અન્તર્ભાવ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#અન્તર્ભાવ <linux/loop.h>
#અન્તર્ભાવ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#અન્તર્ભાવ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#અન્તર્ભાવ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#અન્તર્ભાવ <paths.h>
#અન્તર્ભાવ <sys/param.h>
#અન્તર્ભાવ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/stat.h>
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#અન્તર્ભાવ <sys/sysmacros.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
સ્થિર પૂર્ણાંક check_loop_mounted(અચલ અક્ષર *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#જો defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	સંરચના loop_info64 loopinfo = {0, };
	પૂર્ણાંક loop_fd, ret;

	જો (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		જો (loop_fd < 0)
			પાછા -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		જો (ret < 0)
			પાછા -1;

		જો (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			પાછા 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	પાછા 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
સ્થિર errcode_t check_mntent_file(અચલ અક્ષર *mtab_file, અચલ અક્ષર *file,
				   પૂર્ણાંક *mount_flags, અક્ષર *mtpt, પૂર્ણાંક mtlen)
{
	સંરચના mntent 	*mnt;
	સંરચના stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	પૂર્ણાંક		fd;

	*mount_flags = 0;

	જો ((f = setmntent (mtab_file, "r")) == ખાલી) {
		જો (errno == ENOENT) {
			જો (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				પાછા 0;
			નહીંતર
				પાછા EXT2_ET_NO_MTAB_FILE;
		}
		પાછા errno;
	}
	જો (stat(file, &st_buf) == 0) {
		જો (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} નહીંતર {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	જ્યારે ((mnt = getmntent (f)) != ખાલી) {
		જો (mnt->mnt_fsname[0] != '/')
			ચાલુ;
		જો (strcmp(file, mnt->mnt_fsname) == 0) {
			જો (stat(mnt->mnt_dir, &st_buf) != 0)
				ચાલુ;
			જો (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				ચાલુ;
			}
			તોડો;
		}
		જો (stat(mnt->mnt_fsname, &st_buf) == 0) {
			જો (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				જો (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					જો (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ચાલુ;
					જો (file_rdev == dir_st_buf.st_dev)
						તોડો;
				}
				જો (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					તોડો;
#endif	/* __GNU__ */
			} નહીંતર {
				જો (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					તોડો;
			}
		}
	}

	જો (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		જો (file_rdev && stat("/", &st_buf) == 0) {
			જો (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				જો (mtpt)
					strncpy(mtpt, "/", mtlen);
				જાઓ is_મૂળ;
			}
		}
#endif	/* __GNU__ */
		જાઓ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	જો (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	જો (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	જો (!strcmp(mnt->mnt_dir, "/")) {
is_મૂળ:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		જો (fd < 0) {
			જો (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} નહીંતર
			close(fd);
		(શૂન્ય) unlink(TEST_FILE);
	}

	જો (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	પાછા retval;
}

સ્થિર errcode_t check_mntent(અચલ અક્ષર *file, પૂર્ણાંક *mount_flags,
			      અક્ષર *mtpt, પૂર્ણાંક mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	જો (retval == 0)
		પાછા 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	જો (retval == 0)
		પાછા 0;
#endif /* __linux__ */
#જો defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	પાછા retval;
#નહીંતર
	*mount_flags = 0;
	પાછા 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#નહીંતર
#જો defined(HAVE_GETMNTINFO)

સ્થિર errcode_t check_getmntinfo(અચલ અક્ષર *file, પૂર્ણાંક *mount_flags,
				  અક્ષર *mtpt, પૂર્ણાંક mtlen)
{
	સંરચના statfs *mp;
        પૂર્ણાંક    len, n;
        અચલ  અક્ષર   *s1;
	અક્ષર	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        જો (n == 0)
		પાછા errno;

        len = કદ(_PATH_DEV) - 1;
        s1 = file;
        જો (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        જ્યારે (--n >= 0) {
                s2 = mp->f_mntfromname;
                જો (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                જો (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			તોડો;
		}
                ++mp;
	}
	જો (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	પાછા 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
સ્થિર પૂર્ણાંક is_swap_device(અચલ અક્ષર *file)
{
	FILE		*f;
	અક્ષર		buf[1024], *cp;
	dev_t		file_dev;
	સંરચના stat	st_buf;
	પૂર્ણાંક		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	જો ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	જો (!(f = fopen("/proc/swaps", "r")))
		પાછા 0;
	/* Skip the first line */
	જો (!fgets(buf, કદ(buf), f))
		જાઓ leave;
	જો (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		જાઓ valid_first_લીટી;

	જ્યારે (fgets(buf, કદ(buf), f)) {
valid_first_લીટી:
		જો ((cp = strchr(buf, ' ')) != ખાલી)
			*cp = 0;
		જો ((cp = strchr(buf, '\t')) != ખાલી)
			*cp = 0;
		જો (strcmp(buf, file) == 0) {
			ret++;
			તોડો;
		}
#ifndef __GNU__
		જો (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			તોડો;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	પાછા ret;
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
errcode_t ext2fs_check_mount_point(અચલ અક્ષર *device, પૂર્ણાંક *mount_flags,
				  અક્ષર *mtpt, પૂર્ણાંક mtlen)
{
	errcode_t	retval = 0;
	પૂર્ણાંક 		busy = 0;

	જો (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		જો (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		પાછા 0;
	}
	જો (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		જો (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		પાછા 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		સંરચના stat st_buf;

		જો (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			પૂર્ણાંક fd = open(device, O_RDONLY | O_EXCL);

			જો (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				પાછા 0;
			} નહીંતર જો (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	જો (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		જો (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} નહીંતર {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#નહીંતર
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#નહીંતર
#જો defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	જો (retval)
		પાછા retval;

	જો (busy)
		*mount_flags |= EXT2_MF_BUSY;

	પાછા 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(અચલ અક્ષર *file, પૂર્ણાંક *mount_flags)
{
	પાછા ext2fs_check_mount_point(file, mount_flags, ખાલી, 0);
}

#ifdef DEBUG
પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	પૂર્ણાંક	retval, mount_flags;
	અક્ષર	mntpt[80];

	જો (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, કદ(mntpt));
	જો (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	જો (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	જો (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	જો (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	જો (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	જો (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	જો (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
