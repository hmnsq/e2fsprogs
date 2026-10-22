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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#അന്തര്ഭാവ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#അന്തര്ഭാവ <linux/loop.h>
#അന്തര്ഭാവ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#അന്തര്ഭാവ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#അന്തര്ഭാവ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#അന്തര്ഭാവ <paths.h>
#അന്തര്ഭാവ <sys/param.h>
#അന്തര്ഭാവ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/stat.h>
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#അന്തര്ഭാവ <sys/sysmacros.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_loop_mounted(സ്ഥിരം അക്ഷരം *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#എങ്കിൽ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ഘടന loop_info64 loopinfo = {0, };
	പൂർണ്ണസംഖ്യ loop_fd, ret;

	എങ്കിൽ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		എങ്കിൽ (loop_fd < 0)
			തിരികെ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		എങ്കിൽ (ret < 0)
			തിരികെ -1;

		എങ്കിൽ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			തിരികെ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	തിരികെ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
സ്ഥിതം errcode_t check_mntent_file(സ്ഥിരം അക്ഷരം *mtab_file, സ്ഥിരം അക്ഷരം *file,
				   പൂർണ്ണസംഖ്യ *mount_flags, അക്ഷരം *mtpt, പൂർണ്ണസംഖ്യ mtlen)
{
	ഘടന mntent 	*mnt;
	ഘടന stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	പൂർണ്ണസംഖ്യ		fd;

	*mount_flags = 0;

	എങ്കിൽ ((f = setmntent (mtab_file, "r")) == ശൂന്യ) {
		എങ്കിൽ (errno == ENOENT) {
			എങ്കിൽ (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				തിരികെ 0;
			അല്ലെങ്കിൽ
				തിരികെ EXT2_ET_NO_MTAB_FILE;
		}
		തിരികെ errno;
	}
	എങ്കിൽ (stat(file, &st_buf) == 0) {
		എങ്കിൽ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} അല്ലെങ്കിൽ {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ആകുമ്പോൾ ((mnt = getmntent (f)) != ശൂന്യ) {
		എങ്കിൽ (mnt->mnt_fsname[0] != '/')
			തുടരുക;
		എങ്കിൽ (strcmp(file, mnt->mnt_fsname) == 0) {
			എങ്കിൽ (stat(mnt->mnt_dir, &st_buf) != 0)
				തുടരുക;
			എങ്കിൽ (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				തുടരുക;
			}
			നിർത്തുക;
		}
		എങ്കിൽ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			എങ്കിൽ (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				എങ്കിൽ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					എങ്കിൽ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						തുടരുക;
					എങ്കിൽ (file_rdev == dir_st_buf.st_dev)
						നിർത്തുക;
				}
				എങ്കിൽ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					നിർത്തുക;
#endif	/* __GNU__ */
			} അല്ലെങ്കിൽ {
				എങ്കിൽ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					നിർത്തുക;
			}
		}
	}

	എങ്കിൽ (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		എങ്കിൽ (file_rdev && stat("/", &st_buf) == 0) {
			എങ്കിൽ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				എങ്കിൽ (mtpt)
					strncpy(mtpt, "/", mtlen);
				പോകുക is_വേര്;
			}
		}
#endif	/* __GNU__ */
		പോകുക errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	എങ്കിൽ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	എങ്കിൽ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	എങ്കിൽ (!strcmp(mnt->mnt_dir, "/")) {
is_വേര്:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		എങ്കിൽ (fd < 0) {
			എങ്കിൽ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} അല്ലെങ്കിൽ
			close(fd);
		(ശൂന്യം) unlink(TEST_FILE);
	}

	എങ്കിൽ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	തിരികെ retval;
}

സ്ഥിതം errcode_t check_mntent(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *mount_flags,
			      അക്ഷരം *mtpt, പൂർണ്ണസംഖ്യ mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	എങ്കിൽ (retval == 0)
		തിരികെ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	എങ്കിൽ (retval == 0)
		തിരികെ 0;
#endif /* __linux__ */
#എങ്കിൽ defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	തിരികെ retval;
#അല്ലെങ്കിൽ
	*mount_flags = 0;
	തിരികെ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#അല്ലെങ്കിൽ
#എങ്കിൽ defined(HAVE_GETMNTINFO)

സ്ഥിതം errcode_t check_getmntinfo(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *mount_flags,
				  അക്ഷരം *mtpt, പൂർണ്ണസംഖ്യ mtlen)
{
	ഘടന statfs *mp;
        പൂർണ്ണസംഖ്യ    len, n;
        സ്ഥിരം  അക്ഷരം   *s1;
	അക്ഷരം	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        എങ്കിൽ (n == 0)
		തിരികെ errno;

        len = വലുപ്പം(_PATH_DEV) - 1;
        s1 = file;
        എങ്കിൽ (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ആകുമ്പോൾ (--n >= 0) {
                s2 = mp->f_mntfromname;
                എങ്കിൽ (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                എങ്കിൽ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			നിർത്തുക;
		}
                ++mp;
	}
	എങ്കിൽ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	തിരികെ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ is_swap_device(സ്ഥിരം അക്ഷരം *file)
{
	FILE		*f;
	അക്ഷരം		buf[1024], *cp;
	dev_t		file_dev;
	ഘടന stat	st_buf;
	പൂർണ്ണസംഖ്യ		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	എങ്കിൽ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	എങ്കിൽ (!(f = fopen("/proc/swaps", "r")))
		തിരികെ 0;
	/* Skip the first line */
	എങ്കിൽ (!fgets(buf, വലുപ്പം(buf), f))
		പോകുക leave;
	എങ്കിൽ (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		പോകുക valid_first_വരി;

	ആകുമ്പോൾ (fgets(buf, വലുപ്പം(buf), f)) {
valid_first_വരി:
		എങ്കിൽ ((cp = strchr(buf, ' ')) != ശൂന്യ)
			*cp = 0;
		എങ്കിൽ ((cp = strchr(buf, '\t')) != ശൂന്യ)
			*cp = 0;
		എങ്കിൽ (strcmp(buf, file) == 0) {
			ret++;
			നിർത്തുക;
		}
#ifndef __GNU__
		എങ്കിൽ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			നിർത്തുക;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	തിരികെ ret;
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
errcode_t ext2fs_check_mount_point(സ്ഥിരം അക്ഷരം *device, പൂർണ്ണസംഖ്യ *mount_flags,
				  അക്ഷരം *mtpt, പൂർണ്ണസംഖ്യ mtlen)
{
	errcode_t	retval = 0;
	പൂർണ്ണസംഖ്യ 		busy = 0;

	എങ്കിൽ (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		എങ്കിൽ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		തിരികെ 0;
	}
	എങ്കിൽ (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		എങ്കിൽ (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		തിരികെ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		ഘടന stat st_buf;

		എങ്കിൽ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			പൂർണ്ണസംഖ്യ fd = open(device, O_RDONLY | O_EXCL);

			എങ്കിൽ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				തിരികെ 0;
			} അല്ലെങ്കിൽ എങ്കിൽ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	എങ്കിൽ (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		എങ്കിൽ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} അല്ലെങ്കിൽ {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#അല്ലെങ്കിൽ
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#അല്ലെങ്കിൽ
#എങ്കിൽ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	തിരികെ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *mount_flags)
{
	തിരികെ ext2fs_check_mount_point(file, mount_flags, ശൂന്യ, 0);
}

#ifdef DEBUG
പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ	retval, mount_flags;
	അക്ഷരം	mntpt[80];

	എങ്കിൽ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, വലുപ്പം(mntpt));
	എങ്കിൽ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	എങ്കിൽ (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	എങ്കിൽ (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	എങ്കിൽ (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	എങ്കിൽ (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	എങ്കിൽ (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	എങ്കിൽ (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
