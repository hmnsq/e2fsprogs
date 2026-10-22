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
#यदि_अपरिभाषितम् _BSD_SOURCE
#परिभाषय _BSD_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _DEFAULT_SOURCE
#परिभाषय _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#यदि_परिभाषितम् HAVE_LINUX_FD_H
#अन्तर्भाव <linux/fd.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_LINUX_LOOP_H
#अन्तर्भाव <linux/loop.h>
#अन्तर्भाव <sys/ioctl.h>
#यदि_परिभाषितम् HAVE_LINUX_MAJOR_H
#अन्तर्भाव <linux/major.h>
#यदि_अन्तः /* HAVE_LINUX_MAJOR_H */
#यदि_अन्तः /* HAVE_LINUX_LOOP_H */
#यदि_परिभाषितम् HAVE_MNTENT_H
#अन्तर्भाव <mntent.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_GETMNTINFO
#अन्तर्भाव <paths.h>
#अन्तर्भाव <sys/param.h>
#अन्तर्भाव <sys/mount.h>
#यदि_अन्तः /* HAVE_GETMNTINFO */
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/stat.h>
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_SYSMACROS_H
#अन्तर्भाव <sys/sysmacros.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि_परिभाषितम् HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
स्थिर पूर्णाङ्क check_loop_mounted(ध्रुव वर्ण *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#यदि defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	संरचना loop_info64 loopinfo = {0, };
	पूर्णाङ्क loop_fd, ret;

	यदि (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = उद्घाटय(mnt_fsname, O_RDONLY);
		यदि (loop_fd < 0)
			प्रत्यागम -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		close(loop_fd);
		यदि (ret < 0)
			प्रत्यागम -1;

		यदि (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			प्रत्यागम 1;
	}
#यदि_अन्तः /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	प्रत्यागम 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
स्थिर errcode_t check_mntent_file(ध्रुव वर्ण *mtab_file, ध्रुव वर्ण *file,
				   पूर्णाङ्क *mount_flags, वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	संरचना mntent 	*mnt;
	संरचना stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	FILE 		*f;
	पूर्णाङ्क		fd;

	*mount_flags = 0;

	यदि ((f = setmntent (mtab_file, "r")) == रिक्त) {
		यदि (errno == ENOENT) {
			यदि (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				प्रत्यागम 0;
			अन्यथा
				प्रत्यागम EXT2_ET_NO_MTAB_FILE;
		}
		प्रत्यागम errno;
	}
	यदि (stat(file, &st_buf) == 0) {
		यदि (ext2fsP_is_disk_device(st_buf.st_बहुलकम्)) {
#यदि_अपरिभाषितम् __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#यदि_अन्तः	/* __GNU__ */
		} अन्यथा {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	यावत् ((mnt = getmntent (f)) != रिक्त) {
		यदि (mnt->mnt_fsname[0] != '/')
			अनुवर्त;
		यदि (वाक्यं_तुलय(file, mnt->mnt_fsname) == 0) {
			यदि (stat(mnt->mnt_dir, &st_buf) != 0)
				अनुवर्त;
			यदि (file_rdev && (file_rdev != st_buf.st_dev)) {
#यदि_परिभाषितम् DEBUG
				प्रारूप_लिख("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#यदि_अन्तः /* DEBUG */
				अनुवर्त;
			}
			भङ्ग;
		}
		यदि (stat(mnt->mnt_fsname, &st_buf) == 0) {
			यदि (ext2fsP_is_disk_device(st_buf.st_बहुलकम्)) {
#यदि_अपरिभाषितम् __GNU__
				यदि (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					यदि (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						अनुवर्त;
					यदि (file_rdev == dir_st_buf.st_dev)
						भङ्ग;
				}
				यदि (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					भङ्ग;
#यदि_अन्तः	/* __GNU__ */
			} अन्यथा {
				यदि (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					भङ्ग;
			}
		}
	}

	यदि (mnt == 0) {
#यदि_अपरिभाषितम् __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		यदि (file_rdev && stat("/", &st_buf) == 0) {
			यदि (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				यदि (mtpt)
					strncpy(mtpt, "/", mtlen);
				गच्छ is_मूलम्;
			}
		}
#यदि_अन्तः	/* __GNU__ */
		गच्छ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#यदि_परिभाषितम् MNTOPT_RO
	/* Check to see if the ro option is set */
	यदि (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#यदि_अन्तः

	यदि (mtpt)
		strncpy(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	यदि (!वाक्यं_तुलय(mnt->mnt_dir, "/")) {
is_मूलम्:
#परिभाषय TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = उद्घाटय(TEST_FILE, O_RDWR|O_CREAT, 0600);
		यदि (fd < 0) {
			यदि (errno == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} अन्यथा
			close(fd);
		(शून्य) unlink(TEST_FILE);
	}

	यदि (mnt && mnt->mnt_type &&
	    (!वाक्यं_तुलय(mnt->mnt_type, "ext4") ||
	     !वाक्यं_तुलय(mnt->mnt_type, "ext3") ||
	     !वाक्यं_तुलय(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	प्रत्यागम retval;
}

स्थिर errcode_t check_mntent(ध्रुव वर्ण *file, पूर्णाङ्क *mount_flags,
			      वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	errcode_t	retval;

#यदि_परिभाषितम् DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	यदि (retval == 0)
		प्रत्यागम 0;
#यदि_अन्तः /* DEBUG */
#यदि_परिभाषितम् __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	यदि (retval == 0)
		प्रत्यागम 0;
#यदि_अन्तः /* __linux__ */
#यदि defined(MOUNTED) || defined(_PATH_MOUNTED)
#यदि_अपरिभाषितम् MOUNTED
#परिभाषय MOUNTED _PATH_MOUNTED
#यदि_अन्तः /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	प्रत्यागम retval;
#अन्यथा
	*mount_flags = 0;
	प्रत्यागम 0;
#यदि_अन्तः /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#अन्यथा
#यदि defined(HAVE_GETMNTINFO)

स्थिर errcode_t check_getmntinfo(ध्रुव वर्ण *file, पूर्णाङ्क *mount_flags,
				  वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	संरचना statfs *mp;
        पूर्णाङ्क    len, n;
        ध्रुव  वर्ण   *s1;
	वर्ण	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        यदि (n == 0)
		प्रत्यागम errno;

        len = परिमाण(_PATH_DEV) - 1;
        s1 = file;
        यदि (वाक्ये_तुलय(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        यावत् (--n >= 0) {
                s2 = mp->f_mntfromname;
                यदि (वाक्ये_तुलय(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                यदि (वाक्यं_तुलय(s1, s2) == 0 || वाक्यं_तुलय(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			भङ्ग;
		}
                ++mp;
	}
	यदि (mtpt)
		strncpy(mtpt, mp->f_mntonname, mtlen);
	प्रत्यागम 0;
}
#यदि_अन्तः /* HAVE_GETMNTINFO */
#यदि_अन्तः /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
स्थिर पूर्णाङ्क is_swap_device(ध्रुव वर्ण *file)
{
	FILE		*f;
	वर्ण		buf[1024], *cp;
	dev_t		file_dev;
	संरचना stat	st_buf;
	पूर्णाङ्क		ret = 0;

	file_dev = 0;
#यदि_अपरिभाषितम् __GNU__ /* The GNU hurd is broken with respect to stat devices */
	यदि ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_बहुलकम्))
		file_dev = st_buf.st_rdev;
#यदि_अन्तः	/* __GNU__ */

	यदि (!(f = fopen("/proc/swaps", "r")))
		प्रत्यागम 0;
	/* Skip the first line */
	यदि (!fgets(buf, परिमाण(buf), f))
		गच्छ leave;
	यदि (*buf && वाक्ये_तुलय(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		गच्छ valid_first_पङ्क्तिः;

	यावत् (fgets(buf, परिमाण(buf), f)) {
valid_first_पङ्क्तिः:
		यदि ((cp = strchr(buf, ' ')) != रिक्त)
			*cp = 0;
		यदि ((cp = strchr(buf, '\t')) != रिक्त)
			*cp = 0;
		यदि (वाक्यं_तुलय(buf, file) == 0) {
			ret++;
			भङ्ग;
		}
#यदि_अपरिभाषितम् __GNU__
		यदि (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_बहुलकम्) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			भङ्ग;
		}
#यदि_अन्तः 	/* __GNU__ */
	}

leave:
	fclose(f);
	प्रत्यागम ret;
}


/*
 * ext2fs_check_mount_point() fills determines if the device is
 * mounted or otherwise busy, and fills in mount_flags with one or
 * more of the following flags: EXT2_MF_MOUNTED, EXT2_MF_ISROOT,
 * EXT2_MF_READONLY, EXT2_MF_SWAP, and EXT2_MF_BUSY.  If mtpt is
 * non-NULL, the directory where the device is mounted is copied to
 * where mtpt is pointing, up to mtlen characters.
 */
#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
errcode_t ext2fs_check_mount_point(ध्रुव वर्ण *device, पूर्णाङ्क *mount_flags,
				  वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	errcode_t	retval = 0;
	पूर्णाङ्क 		busy = 0;

	यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		प्रत्यागम 0;
	}
	यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		प्रत्यागम 0;
	}

#यदि_परिभाषितम् __linux__ /* This only works on Linux 2.6+ systems */
	{
		संरचना stat st_buf;

		यदि (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_बहुलकम्)) {
			पूर्णाङ्क fd = उद्घाटय(device, O_RDONLY | O_EXCL);

			यदि (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				close(fd);
				*mount_flags = 0;
				प्रत्यागम 0;
			} अन्यथा यदि (errno == EBUSY) {
				busy = 1;
			}
		}
	}
#यदि_अन्तः

	यदि (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		यदि (mtpt)
			strncpy(mtpt, "<swap>", mtlen);
	} अन्यथा {
#यदि_परिभाषितम् HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#अन्यथा
#यदि_परिभाषितम् HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#अन्यथा
#यदि defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#यदि_अन्तः
		*mount_flags = 0;
#यदि_अन्तः /* HAVE_GETMNTINFO */
#यदि_अन्तः /* HAVE_SETMNTENT */
	}
	यदि (retval)
		प्रत्यागम retval;

	यदि (busy)
		*mount_flags |= EXT2_MF_BUSY;

	प्रत्यागम 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(ध्रुव वर्ण *file, पूर्णाङ्क *mount_flags)
{
	प्रत्यागम ext2fs_check_mount_point(file, mount_flags, रिक्त, 0);
}

#यदि_परिभाषितम् DEBUG
पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	retval, mount_flags;
	वर्ण	mntpt[80];

	यदि (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		निर्गच्छ(1);
	}

	सङ्कलय_error_सारणी(&et_ext2_error_सारणी);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, परिमाण(mntpt));
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("Device %s reports flags %02x\n", argv[1], mount_flags);
	यदि (mount_flags & EXT2_MF_BUSY)
		प्रारूप_लिख("\t%s is apparently in use.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_MOUNTED)
		प्रारूप_लिख("\t%s is mounted.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_SWAP)
		प्रारूप_लिख("\t%s is a swap device.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_READONLY)
		प्रारूप_लिख("\t%s is read-only.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_ISROOT)
		प्रारूप_लिख("\t%s is the root filesystem.\n", argv[1]);
	यदि (mntpt[0])
		प्रारूप_लिख("\t%s is mounted on %s.\n", argv[1], mntpt);
	निर्गच्छ(0);
}
#यदि_अन्तः /* DEBUG */
