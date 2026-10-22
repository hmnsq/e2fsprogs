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
#నిర్వచితం_కాకపోతే BSD_ఆధారం
#నిర్వచించు BSD_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే _DEFAULT_SOURCE
#నిర్వచించు _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#నిర్వచితమైతే HAVE_LINUX_FD_H
#అన్తర్భావ <linux/fd.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_LINUX_LOOP_H
#అన్తర్భావ <linux/loop.h>
#అన్తర్భావ <sys/ioctl.h>
#నిర్వచితమైతే HAVE_LINUX_MAJOR_H
#అన్తర్భావ <linux/major.h>
#అయితే_ముగింపు /* HAVE_LINUX_MAJOR_H */
#అయితే_ముగింపు /* HAVE_LINUX_LOOP_H */
#నిర్వచితమైతే HAVE_MNTENT_H
#అన్తర్భావ <mntent.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_GETMNTINFO
#అన్తర్భావ <paths.h>
#అన్తర్భావ <sys/param.h>
#అన్తర్భావ <sys/mount.h>
#అయితే_ముగింపు /* HAVE_GETMNTINFO */
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/stat.h>
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_SYSMACROS_H
#అన్తర్భావ <sys/sysmacros.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచితమైతే HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
స్థిర పూర్ణాంకం check_loop_mounted(స్థిరరాశి అక్షరం *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#ఒకవేళ defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	నిర్మాణం loop_info64 loopinfo = {0, };
	పూర్ణాంకం loop_fd, ret;

	ఒకవేళ (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = open(mnt_fsname, O_RDONLY);
		ఒకవేళ (loop_fd < 0)
			తిరిగి -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		ఒకవేళ (ret < 0)
			తిరిగి -1;

		ఒకవేళ (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			తిరిగి 1;
	}
#అయితే_ముగింపు /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	తిరిగి 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
స్థిర errcode_t check_mntent_file(స్థిరరాశి అక్షరం *mtab_file, స్థిరరాశి అక్షరం *file,
				   పూర్ణాంకం *mount_flags, అక్షరం *mtpt, పూర్ణాంకం mtlen)
{
	నిర్మాణం mntent 	*mnt;
	నిర్మాణం stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	పూర్ణాంకం		fd;

	*mount_flags = 0;

	ఒకవేళ ((f = setmntent (mtab_file, "r")) == ఖాళీ) {
		ఒకవేళ (errno == ENOENT) {
			ఒకవేళ (ext2fs_సురక్షిత_getenv("EXT2FS_NO_MTAB_OK"))
				తిరిగి 0;
			లేకపోతే
				తిరిగి EXT2_ET_NO_MTAB_FILE;
		}
		తిరిగి errno;
	}
	ఒకవేళ (stat(file, &st_buf) == 0) {
		ఒకవేళ (ext2fsP_is_disk_device(st_buf.st_బహుళకం)) {
#నిర్వచితం_కాకపోతే __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#అయితే_ముగింపు	/* __GNU__ */
		} లేకపోతే {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	అయితే ((mnt = getmntent (f)) != ఖాళీ) {
		ఒకవేళ (mnt->mnt_fsname[0] != '/')
			కొనసాగు;
		ఒకవేళ (strcmp(file, mnt->mnt_fsname) == 0) {
			ఒకవేళ (stat(mnt->mnt_dir, &st_buf) != 0)
				కొనసాగు;
			ఒకవేళ (file_rdev && (file_rdev != st_buf.st_dev)) {
#నిర్వచితమైతే DEBUG
				రాయి("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#అయితే_ముగింపు /* DEBUG */
				కొనసాగు;
			}
			ఆపు;
		}
		ఒకవేళ (stat(mnt->mnt_fsname, &st_buf) == 0) {
			ఒకవేళ (ext2fsP_is_disk_device(st_buf.st_బహుళకం)) {
#నిర్వచితం_కాకపోతే __GNU__
				ఒకవేళ (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					ఒకవేళ (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						కొనసాగు;
					ఒకవేళ (file_rdev == dir_st_buf.st_dev)
						ఆపు;
				}
				ఒకవేళ (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					ఆపు;
#అయితే_ముగింపు	/* __GNU__ */
			} లేకపోతే {
				ఒకవేళ (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					ఆపు;
			}
		}
	}

	ఒకవేళ (mnt == 0) {
#నిర్వచితం_కాకపోతే __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		ఒకవేళ (file_rdev && stat("/", &st_buf) == 0) {
			ఒకవేళ (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				ఒకవేళ (mtpt)
					strncpy(mtpt, "/", mtlen);
				వెళ్ళు is_మూలం;
			}
		}
#అయితే_ముగింపు	/* __GNU__ */
		వెళ్ళు errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#నిర్వచితమైతే MNTOPT_RO
	/* Check to see if the ro option is set */
	ఒకవేళ (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#అయితే_ముగింపు

	ఒకవేళ (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	ఒకవేళ (!strcmp(mnt->mnt_dir, "/")) {
is_మూలం:
#నిర్వచించు TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = open(TEST_FILE, O_RDWR|O_CREAT, 0600);
		ఒకవేళ (fd < 0) {
			ఒకవేళ (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} లేకపోతే
			close(fd);
		(శూన్య) unlink(TEST_FILE);
	}

	ఒకవేళ (mnt && mnt->mnt_type &&
	    (!strcmp(mnt->mnt_type, "ext4") ||
	     !strcmp(mnt->mnt_type, "ext3") ||
	     !strcmp(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	తిరిగి retval;
}

స్థిర errcode_t check_mntent(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *mount_flags,
			      అక్షరం *mtpt, పూర్ణాంకం mtlen)
{
	errcode_t	retval;

#నిర్వచితమైతే DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	ఒకవేళ (retval == 0)
		తిరిగి 0;
#అయితే_ముగింపు /* DEBUG */
#నిర్వచితమైతే __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	ఒకవేళ (retval == 0)
		తిరిగి 0;
#అయితే_ముగింపు /* __linux__ */
#ఒకవేళ defined(MOUNTED) || defined(_PATH_MOUNTED)
#నిర్వచితం_కాకపోతే MOUNTED
#నిర్వచించు MOUNTED _PATH_MOUNTED
#అయితే_ముగింపు /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	తిరిగి retval;
#లేకపోతే
	*mount_flags = 0;
	తిరిగి 0;
#అయితే_ముగింపు /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#లేకపోతే
#ఒకవేళ defined(HAVE_GETMNTINFO)

స్థిర errcode_t check_getmntinfo(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *mount_flags,
				  అక్షరం *mtpt, పూర్ణాంకం mtlen)
{
	నిర్మాణం statfs *mp;
        పూర్ణాంకం    len, n;
        స్థిరరాశి  అక్షరం   *s1;
	అక్షరం	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        ఒకవేళ (n == 0)
		తిరిగి errno;

        len = పరిమాణం(_PATH_DEV) - 1;
        s1 = file;
        ఒకవేళ (అక్షరశ్రేణి_పోలిక(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        అయితే (--n >= 0) {
                s2 = mp->f_mntfromname;
                ఒకవేళ (అక్షరశ్రేణి_పోలిక(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                ఒకవేళ (strcmp(s1, s2) == 0 || strcmp(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			ఆపు;
		}
                ++mp;
	}
	ఒకవేళ (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	తిరిగి 0;
}
#అయితే_ముగింపు /* HAVE_GETMNTINFO */
#అయితే_ముగింపు /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
స్థిర పూర్ణాంకం is_మార్పిడి_device(స్థిరరాశి అక్షరం *file)
{
	FILE		*f;
	అక్షరం		buf[1024], *cp;
	dev_t		file_dev;
	నిర్మాణం stat	st_buf;
	పూర్ణాంకం		ret = 0;

	file_dev = 0;
#నిర్వచితం_కాకపోతే __GNU__ /* The GNU hurd is broken with respect to stat devices */
	ఒకవేళ ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_బహుళకం))
		file_dev = st_buf.st_rdev;
#అయితే_ముగింపు	/* __GNU__ */

	ఒకవేళ (!(f = fopen("/proc/swaps", "r")))
		తిరిగి 0;
	/* Skip the first line */
	ఒకవేళ (!fgets(buf, పరిమాణం(buf), f))
		వెళ్ళు leave;
	ఒకవేళ (*buf && అక్షరశ్రేణి_పోలిక(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		వెళ్ళు valid_మొదటి_పంక్తి;

	అయితే (fgets(buf, పరిమాణం(buf), f)) {
valid_మొదటి_పంక్తి:
		ఒకవేళ ((cp = strchr(buf, ' ')) != ఖాళీ)
			*cp = 0;
		ఒకవేళ ((cp = strchr(buf, '\t')) != ఖాళీ)
			*cp = 0;
		ఒకవేళ (strcmp(buf, file) == 0) {
			ret++;
			ఆపు;
		}
#నిర్వచితం_కాకపోతే __GNU__
		ఒకవేళ (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_బహుళకం) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			ఆపు;
		}
#అయితే_ముగింపు 	/* __GNU__ */
	}

leave:
	fclose(f);
	తిరిగి ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
errcode_t ext2fs_check_mount_బిందువు(స్థిరరాశి అక్షరం *device, పూర్ణాంకం *mount_flags,
				  అక్షరం *mtpt, పూర్ణాంకం mtlen)
{
	errcode_t	retval = 0;
	పూర్ణాంకం 		busy = 0;

	ఒకవేళ (ext2fs_సురక్షిత_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		ఒకవేళ (ext2fs_సురక్షిత_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		తిరిగి 0;
	}
	ఒకవేళ (ext2fs_సురక్షిత_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		ఒకవేళ (ext2fs_సురక్షిత_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		తిరిగి 0;
	}

#నిర్వచితమైతే __linux__ /* This only works on Linux 2.6+ systems */
	{
		నిర్మాణం stat st_buf;

		ఒకవేళ (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_బహుళకం)) {
			పూర్ణాంకం fd = open(device, O_RDONLY | O_EXCL);

			ఒకవేళ (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				తిరిగి 0;
			} లేకపోతే ఒకవేళ (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#అయితే_ముగింపు

	ఒకవేళ (is_మార్పిడి_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_మార్పిడి;
		ఒకవేళ (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} లేకపోతే {
#నిర్వచితమైతే HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#లేకపోతే
#నిర్వచితమైతే HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#లేకపోతే
#ఒకవేళ defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#అయితే_ముగింపు
		*mount_flags = 0;
#అయితే_ముగింపు /* HAVE_GETMNTINFO */
#అయితే_ముగింపు /* HAVE_SETMNTENT */
	}
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (busy)
		*mount_flags |= EXT2_MF_BUSY;

	తిరిగి 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *mount_flags)
{
	తిరిగి ext2fs_check_mount_బిందువు(file, mount_flags, ఖాళీ, 0);
}

#నిర్వచితమైతే DEBUG
పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం	retval, mount_flags;
	అక్షరం	mntpt[80];

	ఒకవేళ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		నిష్క్రమించు(1);
	}

	కూడు_error_పట్టిక(&et_ext2_error_పట్టిక);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_బిందువు(argv[1], &mount_flags,
					  mntpt, పరిమాణం(mntpt));
	ఒకవేళ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		నిష్క్రమించు(1);
	}
	రాయి("Device %s reports flags %02x\n", argv[1], mount_flags);
	ఒకవేళ (mount_flags & EXT2_MF_BUSY)
		రాయి("\t%s is apparently in use.\n", argv[1]);
	ఒకవేళ (mount_flags & EXT2_MF_MOUNTED)
		రాయి("\t%s is mounted.\n", argv[1]);
	ఒకవేళ (mount_flags & EXT2_MF_మార్పిడి)
		రాయి("\t%s is a swap device.\n", argv[1]);
	ఒకవేళ (mount_flags & EXT2_MF_READONLY)
		రాయి("\t%s is read-only.\n", argv[1]);
	ఒకవేళ (mount_flags & EXT2_MF_ISROOT)
		రాయి("\t%s is the root filesystem.\n", argv[1]);
	ఒకవేళ (mntpt[0])
		రాయి("\t%s is mounted on %s.\n", argv[1], mntpt);
	నిష్క్రమించు(0);
}
#అయితే_ముగింపు /* DEBUG */
