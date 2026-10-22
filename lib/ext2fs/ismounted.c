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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#ifdef HAVE_LINUX_FD_H
#अन्तर्भाव <linux/fd.h>
#endif
#ifdef HAVE_LINUX_LOOP_H
#अन्तर्भाव <linux/loop.h>
#अन्तर्भाव <sys/ioctl.h>
#ifdef HAVE_LINUX_MAJOR_H
#अन्तर्भाव <linux/major.h>
#endif /* HAVE_LINUX_MAJOR_H */
#endif /* HAVE_LINUX_LOOP_H */
#ifdef HAVE_MNTENT_H
#अन्तर्भाव <mntent.h>
#endif
#ifdef HAVE_GETMNTINFO
#अन्तर्भाव <paths.h>
#अन्तर्भाव <sys/param.h>
#अन्तर्भाव <sys/mount.h>
#endif /* HAVE_GETMNTINFO */
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/stat.h>
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#अन्तर्भाव <sys/sysmacros.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#ifdef HAVE_SETMNTENT
/*
 * Check to see if a regular file is mounted.
 * If /etc/mtab/ is a symlink of /proc/mounts, you will need the following check
 * because the name in /proc/mounts is a loopback device not a regular file.
 */
स्थिर पूर्णांक check_loop_mounted(स्थिरांक अक्षर *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#जर defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	संरचना loop_info64 loopinfo = {0, };
	पूर्णांक loop_fd, ret;

	जर (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = खोलो(mnt_fsname, O_RDONLY);
		जर (loop_fd < 0)
			परत -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		बंद(loop_fd);
		जर (ret < 0)
			परत -1;

		जर (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			परत 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	परत 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
स्थिर errcode_t check_mntent_file(स्थिरांक अक्षर *mtab_file, स्थिरांक अक्षर *file,
				   पूर्णांक *mount_flags, अक्षर *mtpt, पूर्णांक mtlen)
{
	संरचना mntent 	*mnt;
	संरचना stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	खाता 		*f;
	पूर्णांक		fd;

	*mount_flags = 0;

	जर ((f = setmntent (mtab_file, "r")) == रिक्त) {
		जर (त्रुटि_सं == ENOENT) {
			जर (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				परत 0;
			अन्यथा
				परत EXT2_ET_NO_MTAB_FILE;
		}
		परत त्रुटि_सं;
	}
	जर (stat(file, &st_buf) == 0) {
		जर (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} अन्यथा {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	पर्यंत ((mnt = getmntent (f)) != रिक्त) {
		जर (mnt->mnt_fsname[0] != '/')
			सुरूठेवा;
		जर (म_भेद(file, mnt->mnt_fsname) == 0) {
			जर (stat(mnt->mnt_dir, &st_buf) != 0)
				सुरूठेवा;
			जर (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				म_लिखो("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				सुरूठेवा;
			}
			थांबा;
		}
		जर (stat(mnt->mnt_fsname, &st_buf) == 0) {
			जर (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				जर (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					जर (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						सुरूठेवा;
					जर (file_rdev == dir_st_buf.st_dev)
						थांबा;
				}
				जर (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					थांबा;
#endif	/* __GNU__ */
			} अन्यथा {
				जर (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					थांबा;
			}
		}
	}

	जर (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
		/*
		 * Do an extra check to see if this is the root device.  We
		 * can't trust /etc/mtab, and /proc/mounts will only list
		 * /dev/root for the root filesystem.  Argh.  Instead we
		 * check if the given device has the same major/minor number
		 * as the device that the root directory is on.
		 */
		जर (file_rdev && stat("/", &st_buf) == 0) {
			जर (st_buf.st_dev == file_rdev) {
				*mount_flags = EXT2_MF_MOUNTED;
				जर (mtpt)
					म_नकलन(mtpt, "/", mtlen);
				जा is_मूळ;
			}
		}
#endif	/* __GNU__ */
		जा errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	जर (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	जर (mtpt)
		म_नकलन(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	जर (!म_भेद(mnt->mnt_dir, "/")) {
is_मूळ:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = खोलो(TEST_FILE, O_RDWR|O_CREAT, 0600);
		जर (fd < 0) {
			जर (त्रुटि_सं == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} अन्यथा
			बंद(fd);
		(शून्य) unlink(TEST_FILE);
	}

	जर (mnt && mnt->mnt_type &&
	    (!म_भेद(mnt->mnt_type, "ext4") ||
	     !म_भेद(mnt->mnt_type, "ext3") ||
	     !म_भेद(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	परत retval;
}

स्थिर errcode_t check_mntent(स्थिरांक अक्षर *file, पूर्णांक *mount_flags,
			      अक्षर *mtpt, पूर्णांक mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	जर (retval == 0)
		परत 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	जर (retval == 0)
		परत 0;
#endif /* __linux__ */
#जर defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	परत retval;
#अन्यथा
	*mount_flags = 0;
	परत 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#अन्यथा
#जर defined(HAVE_GETMNTINFO)

स्थिर errcode_t check_getmntinfo(स्थिरांक अक्षर *file, पूर्णांक *mount_flags,
				  अक्षर *mtpt, पूर्णांक mtlen)
{
	संरचना statfs *mp;
        पूर्णांक    len, n;
        स्थिरांक  अक्षर   *s1;
	अक्षर	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        जर (n == 0)
		परत त्रुटि_सं;

        len = आकार(_PATH_DEV) - 1;
        s1 = file;
        जर (म_भेदन(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        पर्यंत (--n >= 0) {
                s2 = mp->f_mntfromname;
                जर (म_भेदन(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                जर (म_भेद(s1, s2) == 0 || म_भेद(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			थांबा;
		}
                ++mp;
	}
	जर (mtpt)
		म_नकलन(mtpt, mp->f_mntonname, mtlen);
	परत 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
स्थिर पूर्णांक is_swap_device(स्थिरांक अक्षर *file)
{
	खाता		*f;
	अक्षर		buf[1024], *cp;
	dev_t		file_dev;
	संरचना stat	st_buf;
	पूर्णांक		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	जर ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	जर (!(f = ख_खोलो("/proc/swaps", "r")))
		परत 0;
	/* Skip the first line */
	जर (!ख_माला_लो(buf, आकार(buf), f))
		जा leave;
	जर (*buf && म_भेदन(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		जा valid_first_ओळ;

	पर्यंत (ख_माला_लो(buf, आकार(buf), f)) {
valid_first_ओळ:
		जर ((cp = म_अक्षर(buf, ' ')) != रिक्त)
			*cp = 0;
		जर ((cp = म_अक्षर(buf, '\t')) != रिक्त)
			*cp = 0;
		जर (म_भेद(buf, file) == 0) {
			ret++;
			थांबा;
		}
#ifndef __GNU__
		जर (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			थांबा;
		}
#endif 	/* __GNU__ */
	}

leave:
	ख_बंद(f);
	परत ret;
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
errcode_t ext2fs_check_mount_point(स्थिरांक अक्षर *device, पूर्णांक *mount_flags,
				  अक्षर *mtpt, पूर्णांक mtlen)
{
	errcode_t	retval = 0;
	पूर्णांक 		busy = 0;

	जर (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		जर (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		परत 0;
	}
	जर (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		जर (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		परत 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		संरचना stat st_buf;

		जर (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			पूर्णांक fd = खोलो(device, O_RDONLY | O_EXCL);

			जर (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				बंद(fd);
				*mount_flags = 0;
				परत 0;
			} अन्यथा जर (त्रुटि_सं == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	जर (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		जर (mtpt)
			म_नकलन(mtpt, "<swap>", mtlen);
	} अन्यथा {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#अन्यथा
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#अन्यथा
#जर defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	जर (retval)
		परत retval;

	जर (busy)
		*mount_flags |= EXT2_MF_BUSY;

	परत 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(स्थिरांक अक्षर *file, पूर्णांक *mount_flags)
{
	परत ext2fs_check_mount_point(file, mount_flags, रिक्त, 0);
}

#ifdef DEBUG
पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक	retval, mount_flags;
	अक्षर	mntpt[80];

	जर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, आकार(mntpt));
	जर (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		निकास(1);
	}
	म_लिखो("Device %s reports flags %02x\n", argv[1], mount_flags);
	जर (mount_flags & EXT2_MF_BUSY)
		म_लिखो("\t%s is apparently in use.\n", argv[1]);
	जर (mount_flags & EXT2_MF_MOUNTED)
		म_लिखो("\t%s is mounted.\n", argv[1]);
	जर (mount_flags & EXT2_MF_SWAP)
		म_लिखो("\t%s is a swap device.\n", argv[1]);
	जर (mount_flags & EXT2_MF_READONLY)
		म_लिखो("\t%s is read-only.\n", argv[1]);
	जर (mount_flags & EXT2_MF_ISROOT)
		म_लिखो("\t%s is the root filesystem.\n", argv[1]);
	जर (mntpt[0])
		म_लिखो("\t%s is mounted on %s.\n", argv[1], mntpt);
	निकास(0);
}
#endif /* DEBUG */
