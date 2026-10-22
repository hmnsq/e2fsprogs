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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#ଅନ୍ତର୍ଭାଵ <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#ଅନ୍ତର୍ଭାଵ <linux/loop.h>
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#ଅନ୍ତର୍ଭାଵ <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#ଅନ୍ତର୍ଭାଵ <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#ଅନ୍ତର୍ଭାଵ <paths.h>
#ଅନ୍ତର୍ଭାଵ <sys/param.h>
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_loop_mounted(ଅଚଳ ଅକ୍ଷର *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ଯଦି defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	ସଂରଚନା loop_info64 loopinfo = {0, };
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା loop_fd, ret;

	ଯଦି (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ଯଦି (loop_fd < 0)
			ଫେରନ୍ତୁ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ଯଦି (ret < 0)
			ଫେରନ୍ତୁ -1;

		ଯଦି (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			ଫେରନ୍ତୁ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	ଫେରନ୍ତୁ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
ସ୍ଥିର errcode_t check_mntent_file(ଅଚଳ ଅକ୍ଷର *mtab_file, ଅଚଳ ଅକ୍ଷର *file,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags, ଅକ୍ଷର *mtpt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtlen)
{
	ସଂରଚନା mntent 	*mnt;
	ସଂରଚନା stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		fd;

	*mount_flags = 0;

	ଯଦି ((f = setmntent (mtab_file, "r")) == ଖାଲି) {
		ଯଦି (errno == ENOENT) {
			ଯଦି (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				ଫେରନ୍ତୁ 0;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ EXT2_ET_NO_MTAB_FILE;
		}
		ଫେରନ୍ତୁ errno;
	}
	ଯଦି (stat(file, &st_buf) == 0) {
		ଯଦି (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} ଅନ୍ୟଥା {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	ଯେତେବେଳେ ((mnt = getmntent (f)) != ଖାଲି) {
		ଯଦି (mnt->mnt_fsname[0] != '/')
			ଜାରି;
		ଯଦି (strcmp(file, mnt->mnt_fsname) == 0) {
			ଯଦି (stat(mnt->mnt_dir, &st_buf) != 0)
				ଜାରି;
			ଯଦି (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				printf("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				ଜାରି;
			}
			ଭାଙ୍ଗ;
		}
		ଯଦି (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ଯଦି (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				ଯଦି (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ଯଦି (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						ଜାରି;
					ଯଦି (file_rdev == dir_st_buf.st_dev)
						ଭାଙ୍ଗ;
				}
				ଯଦି (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ଭାଙ୍ଗ;
#endif	/* __GNU__ */
			} ଅନ୍ୟଥା {
				ଯଦି (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ଭାଙ୍ଗ;
			}
		}
	}

	ଯଦି (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ଯଦି (file_rdev && stat("/", &st_buf) == 0) {
			ଯଦି (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ଯଦି (mtpt)
					strncpy(mtpt, "/", mtlen);
				ଯାଅ is_ମୂଳ;
			}
		}
#endif	/* __GNU__ */
		ଯାଅ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	ଯଦି (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	ଯଦି (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ଯଦି (!strcmp(mnt->mnt_dir, "/")) {
is_ମୂଳ:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ଯଦି (fd < 0) {
			ଯଦି (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} ଅନ୍ୟଥା
			close(fd);
		(ଶୂନ୍ୟ) unlink(TEST_FILE);
	}

	ଯଦି (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t check_mntent(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags,
			      ଅକ୍ଷର *mtpt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ଯଦି (retval == 0)
		ଫେରନ୍ତୁ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ଯଦି (retval == 0)
		ଫେରନ୍ତୁ 0;
#endif /* __linux__ */
#ଯଦି defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	*mount_flags = 0;
	ଫେରନ୍ତୁ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#ଅନ୍ୟଥା
#ଯଦି defined(HAVE_GETMNTINFO)

ସ୍ଥିର errcode_t check_getmntinfo(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags,
				  ଅକ୍ଷର *mtpt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtlen)
{
	ସଂରଚନା statfs *mp;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା    len, n;
        ଅଚଳ  ଅକ୍ଷର   *s1;
	ଅକ୍ଷର	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ଯଦି (n == 0)
		ଫେରନ୍ତୁ errno;

        len = ଆକାର(_PATH_DEV) - 1;
        s1 = file;
        ଯଦି (strncmp(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        ଯେତେବେଳେ (--n >= 0) {
                s2 = mp->f_mntfromname;
                ଯଦି (strncmp(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ଯଦି (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ଭାଙ୍ଗ;
		}
                ++mp;
	}
	ଯଦି (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	ଫେରନ୍ତୁ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_swap_device(ଅଚଳ ଅକ୍ଷର *file)
{
	FILE		*f;
	ଅକ୍ଷର		buf[1024], *cp;
	dev_t		file_dev;
	ସଂରଚନା stat	st_buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ଯଦି ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	ଯଦି (!(f = fopen("/proc/swaps", "r")))
		ଫେରନ୍ତୁ 0;
	/* Skip the first line */
	ଯଦି (!fgets(buf, ଆକାର(buf), f))
		ଯାଅ leave;
	ଯଦି (*buf && strncmp(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		ଯାଅ valid_first_ଧାଡ଼ି;

	ଯେତେବେଳେ (fgets(buf, ଆକାର(buf), f)) {
valid_first_ଧାଡ଼ି:
		ଯଦି ((cp = strchr(buf, ' ')) != ଖାଲି)
			*cp = 0;
		ଯଦି ((cp = strchr(buf, '\t')) != ଖାଲି)
			*cp = 0;
		ଯଦି (strcmp(buf, file) == 0) {
			ret++;
			ଭାଙ୍ଗ;
		}
#ifndef __GNU__
		ଯଦି (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ଭାଙ୍ଗ;
		}
#endif 	/* __GNU__ */
	}

leave:
	fclose(f);
	ଫେରନ୍ତୁ ret;
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
errcode_t ext2fs_check_mount_point(ଅଚଳ ଅକ୍ଷର *device, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags,
				  ଅକ୍ଷର *mtpt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtlen)
{
	errcode_t	retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା 		busy = 0;

	ଯଦି (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ଯଦି (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ଯଦି (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		ଫେରନ୍ତୁ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		ସଂରଚନା stat st_buf;

		ଯଦି (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(device, O_RDONLY | O_EXCL);

			ଯଦି (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				ଫେରନ୍ତୁ 0;
			} ଅନ୍ୟଥା ଯଦି (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	ଯଦି (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		ଯଦି (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} ଅନ୍ୟଥା {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#ଅନ୍ୟଥା
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#ଅନ୍ୟଥା
#ଯଦି defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (busy)
		*mount_flags |= EXT2_MF_BUSY;

	ଫେରନ୍ତୁ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags)
{
	ଫେରନ୍ତୁ ext2fs_check_mount_point(file, mount_flags, ଖାଲି, 0);
}

#ifdef DEBUG
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	retval, mount_flags;
	ଅକ୍ଷର	mntpt[80];

	ଯଦି (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, ଆକାର(mntpt));
	ଯଦି (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		exit(1);
	}
	printf("Device %s reports flags %02x\n", argv[1], mount_flags);
	ଯଦି (mount_flags & EXT2_MF_BUSY)
		printf("\t%s is apparently in use.\n", argv[1]);
	ଯଦି (mount_flags & EXT2_MF_MOUNTED)
		printf("\t%s is mounted.\n", argv[1]);
	ଯଦି (mount_flags & EXT2_MF_SWAP)
		printf("\t%s is a swap device.\n", argv[1]);
	ଯଦି (mount_flags & EXT2_MF_READONLY)
		printf("\t%s is read-only.\n", argv[1]);
	ଯଦି (mount_flags & EXT2_MF_ISROOT)
		printf("\t%s is the root filesystem.\n", argv[1]);
	ଯଦି (mntpt[0])
		printf("\t%s is mounted on %s.\n", argv[1], mntpt);
	exit(0);
}
#endif /* DEBUG */
