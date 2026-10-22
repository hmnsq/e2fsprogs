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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
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
#यदि HAVE_SYS_TYPES_H
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
स्थिर पूर्णाङ्क check_loop_mounted(अचर वर्ण *mnt_fsname, dev_t mnt_rdev,
				dev_t file_dev, ino_t file_ino)
{
#यदि defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H)
	संरचना loop_info64 loopinfo = {0, };
	पूर्णाङ्क loop_fd, ret;

	यदि (major(mnt_rdev) == LOOP_MAJOR) {
		loop_fd = खोलो(mnt_fsname, O_RDONLY);
		यदि (loop_fd < 0)
			फर्काऊ -1;

		ret = ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo);
		बंद(loop_fd);
		यदि (ret < 0)
			फर्काऊ -1;

		यदि (file_dev == loopinfo.lo_device &&
				file_ino == loopinfo.lo_inode)
			फर्काऊ 1;
	}
#endif /* defined(HAVE_LINUX_LOOP_H) && defined(HAVE_LINUX_MAJOR_H) */
	फर्काऊ 0;
}

/*
 * Helper function which checks a file in /etc/mtab format to see if a
 * filesystem is mounted.  Returns an error if the file doesn't exist
 * or can't be opened.
 */
स्थिर errcode_t check_mntent_file(अचर वर्ण *mtab_file, अचर वर्ण *file,
				   पूर्णाङ्क *mount_flags, वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	संरचना mntent 	*mnt;
	संरचना stat	st_buf, dir_st_buf;
	errcode_t	retval = 0;
	dev_t		file_dev=0, file_rdev=0;
	ino_t		file_ino=0;
	खाता 		*f;
	पूर्णाङ्क		fd;

	*mount_flags = 0;

	यदि ((f = setmntent (mtab_file, "r")) == रिक्त) {
		यदि (त्रुटि_सं == ENOENT) {
			यदि (ext2fs_safe_getenv("EXT2FS_NO_MTAB_OK"))
				फर्काऊ 0;
			अन्यथा
				फर्काऊ EXT2_ET_NO_MTAB_FILE;
		}
		फर्काऊ त्रुटि_सं;
	}
	यदि (stat(file, &st_buf) == 0) {
		यदि (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
			file_rdev = st_buf.st_rdev;
#endif	/* __GNU__ */
		} अन्यथा {
			file_dev = st_buf.st_dev;
			file_ino = st_buf.st_ino;
		}
	}
	जबसम्म ((mnt = getmntent (f)) != रिक्त) {
		यदि (mnt->mnt_fsname[0] != '/')
			जारी;
		यदि (म_भेद(file, mnt->mnt_fsname) == 0) {
			यदि (stat(mnt->mnt_dir, &st_buf) != 0)
				जारी;
			यदि (file_rdev && (file_rdev != st_buf.st_dev)) {
#ifdef DEBUG
				म_लिखो("Bogus entry in %s!  "
				       "(%s is not mounted on %s)\n",
				       mtab_file, file, mnt->mnt_dir);
#endif /* DEBUG */
				जारी;
			}
			रोक;
		}
		यदि (stat(mnt->mnt_fsname, &st_buf) == 0) {
			यदि (ext2fsP_is_disk_device(st_buf.st_mode)) {
#ifndef __GNU__
				यदि (file_rdev &&
				    (file_rdev == st_buf.st_rdev)) {
					यदि (stat(mnt->mnt_dir,
						 &dir_st_buf) != 0)
						जारी;
					यदि (file_rdev == dir_st_buf.st_dev)
						रोक;
				}
				यदि (check_loop_mounted(mnt->mnt_fsname,
						st_buf.st_rdev, file_dev,
						file_ino) == 1)
					रोक;
#endif	/* __GNU__ */
			} अन्यथा {
				यदि (file_dev && ((file_dev == st_buf.st_dev) &&
						 (file_ino == st_buf.st_ino)))
					रोक;
			}
		}
	}

	यदि (mnt == 0) {
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
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
					म_नकलन(mtpt, "/", mtlen);
				गच्छ is_जरा;
			}
		}
#endif	/* __GNU__ */
		गच्छ errout;
	}
	*mount_flags = EXT2_MF_MOUNTED;

#ifdef MNTOPT_RO
	/* Check to see if the ro option is set */
	यदि (hasmntopt(mnt, MNTOPT_RO))
		*mount_flags |= EXT2_MF_READONLY;
#endif

	यदि (mtpt)
		म_नकलन(mtpt, mnt->mnt_dir, mtlen);
	/*
	 * Check to see if we're referring to the root filesystem.
	 * If so, do a manual check to see if we can open /etc/mtab
	 * read/write, since if the root is mounted read/only, the
	 * contents of /etc/mtab may not be accurate.
	 */
	यदि (!म_भेद(mnt->mnt_dir, "/")) {
is_जरा:
#define TEST_FILE "/.ismount-test-file"
		*mount_flags |= EXT2_MF_ISROOT;
		fd = खोलो(TEST_FILE, O_RDWR|O_CREAT, 0600);
		यदि (fd < 0) {
			यदि (त्रुटि_सं == EROFS)
				*mount_flags |= EXT2_MF_READONLY;
		} अन्यथा
			बंद(fd);
		(शून्य) unlink(TEST_FILE);
	}

	यदि (mnt && mnt->mnt_type &&
	    (!म_भेद(mnt->mnt_type, "ext4") ||
	     !म_भेद(mnt->mnt_type, "ext3") ||
	     !म_भेद(mnt->mnt_type, "ext2")))
		*mount_flags |= EXT2_MF_EXTFS;
	retval = 0;
errout:
	endmntent (f);
	फर्काऊ retval;
}

स्थिर errcode_t check_mntent(अचर वर्ण *file, पूर्णाङ्क *mount_flags,
			      वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	errcode_t	retval;

#ifdef DEBUG
	retval = check_mntent_file("/tmp/mtab", file, mount_flags,
				   mtpt, mtlen);
	यदि (retval == 0)
		फर्काऊ 0;
#endif /* DEBUG */
#ifdef __linux__
	retval = check_mntent_file("/proc/mounts", file, mount_flags,
				   mtpt, mtlen);
	यदि (retval == 0)
		फर्काऊ 0;
#endif /* __linux__ */
#यदि defined(MOUNTED) || defined(_PATH_MOUNTED)
#ifndef MOUNTED
#define MOUNTED _PATH_MOUNTED
#endif /* MOUNTED */
	retval = check_mntent_file(MOUNTED, file, mount_flags, mtpt, mtlen);
	फर्काऊ retval;
#अन्यथा
	*mount_flags = 0;
	फर्काऊ 0;
#endif /* defined(MOUNTED) || defined(_PATH_MOUNTED) */
}

#अन्यथा
#यदि defined(HAVE_GETMNTINFO)

स्थिर errcode_t check_getmntinfo(अचर वर्ण *file, पूर्णाङ्क *mount_flags,
				  वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	संरचना statfs *mp;
        पूर्णाङ्क    len, n;
        अचर  वर्ण   *s1;
	वर्ण	*s2;

        n = getmntinfo(&mp, MNT_NOWAIT);
        यदि (n == 0)
		फर्काऊ त्रुटि_सं;

        len = परिमाण(_PATH_DEV) - 1;
        s1 = file;
        यदि (म_भेदन(_PATH_DEV, s1, len) == 0)
                s1 += len;

	*mount_flags = 0;
        जबसम्म (--n >= 0) {
                s2 = mp->f_mntfromname;
                यदि (म_भेदन(_PATH_DEV, s2, len) == 0) {
                        s2 += len - 1;
                        *s2 = 'r';
                }
                यदि (म_भेद(s1, s2) == 0 || म_भेद(s1, &s2[1]) == 0) {
			*mount_flags = EXT2_MF_MOUNTED;
			रोक;
		}
                ++mp;
	}
	यदि (mtpt)
		म_नकलन(mtpt, mp->f_mntonname, mtlen);
	फर्काऊ 0;
}
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */

/*
 * Check to see if we're dealing with the swap device.
 */
स्थिर पूर्णाङ्क is_swap_device(अचर वर्ण *file)
{
	खाता		*f;
	वर्ण		buf[1024], *cp;
	dev_t		file_dev;
	संरचना stat	st_buf;
	पूर्णाङ्क		ret = 0;

	file_dev = 0;
#ifndef __GNU__ /* The GNU hurd is broken with respect to stat devices */
	यदि ((stat(file, &st_buf) == 0) &&
	    ext2fsP_is_disk_device(st_buf.st_mode))
		file_dev = st_buf.st_rdev;
#endif	/* __GNU__ */

	यदि (!(f = ख_खोलो("/proc/swaps", "r")))
		फर्काऊ 0;
	/* Skip the first line */
	यदि (!ख_माला_लो(buf, परिमाण(buf), f))
		गच्छ leave;
	यदि (*buf && म_भेदन(buf, "Filename\t", 9))
		/* Linux <=2.6.19 contained a bug in the /proc/swaps
		 * code where the header would not be displayed
		 */
		गच्छ valid_first_पङ्क्ति;

	जबसम्म (ख_माला_लो(buf, परिमाण(buf), f)) {
valid_first_पङ्क्ति:
		यदि ((cp = म_अक्षर(buf, ' ')) != रिक्त)
			*cp = 0;
		यदि ((cp = म_अक्षर(buf, '\t')) != रिक्त)
			*cp = 0;
		यदि (म_भेद(buf, file) == 0) {
			ret++;
			रोक;
		}
#ifndef __GNU__
		यदि (file_dev && (stat(buf, &st_buf) == 0) &&
		    ext2fsP_is_disk_device(st_buf.st_mode) &&
		    file_dev == st_buf.st_rdev) {
			ret++;
			रोक;
		}
#endif 	/* __GNU__ */
	}

leave:
	ख_बंद(f);
	फर्काऊ ret;
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
errcode_t ext2fs_check_mount_point(अचर वर्ण *device, पूर्णाङ्क *mount_flags,
				  वर्ण *mtpt, पूर्णाङ्क mtlen)
{
	errcode_t	retval = 0;
	पूर्णाङ्क 		busy = 0;

	यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_RO_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_READONLY;
		यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		फर्काऊ 0;
	}
	यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_RW_MOUNT")) {
		*mount_flags = EXT2_MF_MOUNTED;
		यदि (ext2fs_safe_getenv("EXT2FS_PRETEND_ROOTFS"))
			*mount_flags = EXT2_MF_ISROOT;
		फर्काऊ 0;
	}

#ifdef __linux__ /* This only works on Linux 2.6+ systems */
	{
		संरचना stat st_buf;

		यदि (stat(device, &st_buf) == 0 &&
		    ext2fsP_is_disk_device(st_buf.st_mode)) {
			पूर्णाङ्क fd = खोलो(device, O_RDONLY | O_EXCL);

			यदि (fd >= 0) {
				/*
				 * The device is not busy so it's
				 * definitelly not mounted. No need to
				 * to perform any more checks.
				 */
				बंद(fd);
				*mount_flags = 0;
				फर्काऊ 0;
			} अन्यथा यदि (त्रुटि_सं == EBUSY) {
				busy = 1;
			}
		}
	}
#endif

	यदि (is_swap_device(device)) {
		*mount_flags = EXT2_MF_MOUNTED | EXT2_MF_SWAP;
		यदि (mtpt)
			म_नकलन(mtpt, "<swap>", mtlen);
	} अन्यथा {
#ifdef HAVE_SETMNTENT
		retval = check_mntent(device, mount_flags, mtpt, mtlen);
#अन्यथा
#ifdef HAVE_GETMNTINFO
		retval = check_getmntinfo(device, mount_flags, mtpt, mtlen);
#अन्यथा
#यदि defined(__GNUC__) && !defined(_WIN32)
 #warning "Can't use getmntent or getmntinfo to check for mounted filesystems!"
#endif
		*mount_flags = 0;
#endif /* HAVE_GETMNTINFO */
#endif /* HAVE_SETMNTENT */
	}
	यदि (retval)
		फर्काऊ retval;

	यदि (busy)
		*mount_flags |= EXT2_MF_BUSY;

	फर्काऊ 0;
}

/*
 * ext2fs_check_if_mounted() sets the mount_flags EXT2_MF_MOUNTED,
 * EXT2_MF_READONLY, and EXT2_MF_ROOT
 *
 */
errcode_t ext2fs_check_if_mounted(अचर वर्ण *file, पूर्णाङ्क *mount_flags)
{
	फर्काऊ ext2fs_check_mount_point(file, mount_flags, रिक्त, 0);
}

#ifdef DEBUG
पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	retval, mount_flags;
	वर्ण	mntpt[80];

	यदि (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	add_error_table(&et_ext2_error_table);
	mntpt[0] = 0;
	retval = ext2fs_check_mount_point(argv[1], &mount_flags,
					  mntpt, परिमाण(mntpt));
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_check_if_mounted");
		निकास(1);
	}
	म_लिखो("Device %s reports flags %02x\n", argv[1], mount_flags);
	यदि (mount_flags & EXT2_MF_BUSY)
		म_लिखो("\t%s is apparently in use.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_MOUNTED)
		म_लिखो("\t%s is mounted.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_SWAP)
		म_लिखो("\t%s is a swap device.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_READONLY)
		म_लिखो("\t%s is read-only.\n", argv[1]);
	यदि (mount_flags & EXT2_MF_ISROOT)
		म_लिखो("\t%s is the root filesystem.\n", argv[1]);
	यदि (mntpt[0])
		म_लिखो("\t%s is mounted on %s.\n", argv[1], mntpt);
	निकास(0);
}
#endif /* DEBUG */
