/*
 * finddev.c -- this routine attempts to find a particular device in
 * 	/dev
 *
 * Copyright (C) 2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <stdlib.h>
#ضمن <string.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#ضمن <dirent.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_MKDEV_H
#ضمن <sys/mkdev.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية dir_list {
	حرف	*name;
	بنية dir_list *ايندڙ;
};

/*
 * This function adds an entry to the directory list
 */
ساكن فراغ جوڙيو_منزل_dirlist(ثابت حرف *name, بنية dir_list **list)
{
	بنية dir_list *dp;

	dp = يادگيري_ڏيو(حجم(بنية dir_list));
	إذا (!dp)
		أرجع;
	dp->name = يادگيري_ڏيو(لڙي_ڊيگهه(name)+1);
	إذا (!dp->name) {
		حرر(dp);
		أرجع;
	}
	strcpy(dp->name, name);
	dp->ايندڙ = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساكن فراغ free_dirlist(بنية dir_list **list)
{
	بنية dir_list *dp, *ايندڙ;

	لكل (dp = *list; dp; dp = ايندڙ) {
		ايندڙ = dp->ايندڙ;
		حرر(dp->name);
		حرر(dp);
	}
	*list = 0;
}

ساكن صحيح جاچ_dir(حرف *dirname, dev_t device, بنية dir_list **list,
		    حرف **ret_رستو)
{
	DIR	*dir;
	بنية dirent *dp;
	حرف	رستو[1024], *cp;
	صحيح	dirlen;
	بنية stat st;

	dirlen = لڙي_ڊيگهه(dirname);
	إذا ((dir = opendir(dirname)) == عدم)
		أرجع errno;
	dp = readdir(dir);
	طالما (dp) {
		إذا (dirlen + لڙي_ڊيگهه(dp->d_name) + 2 >= حجم(رستو))
			اذهب skip_منزل_ايندڙ;
		إذا (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			اذهب skip_منزل_ايندڙ;
		sprintf(رستو, "%s/%s", dirname, dp->d_name);
		إذا (stat(رستو, &st) < 0)
			اذهب skip_منزل_ايندڙ;
		إذا (S_ISDIR(st.st_گهڻائي))
			جوڙيو_منزل_dirlist(رستو, list);
		إذا (ext2fsP_is_disk_device(st.st_گهڻائي) &&
		    st.st_rdev == device) {
			cp = يادگيري_ڏيو(لڙي_ڊيگهه(رستو)+1);
			إذا (!cp) {
				closedir(dir);
				أرجع ENOMEM;
			}
			strcpy(cp, رستو);
			*ret_رستو = cp;
			اذهب success;
		}
	skip_منزل_ايندڙ:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	أرجع 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
حرف *ext2fs_ڳوليو_بلاڪ_device(dev_t device)
{
	بنية dir_list *list = 0, *new_list = 0;
	بنية dir_list *current;
	حرف	*ret_رستو = 0;
	صحيح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	جوڙيو_منزل_dirlist("/devices", &list);
	جوڙيو_منزل_dirlist("/devfs", &list);
	جوڙيو_منزل_dirlist("/dev", &list);

	طالما (list) {
		current = list;
		list = list->ايندڙ;
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("Scanning directory %s\n", current->name);
#جيڪڏهن_پورو
		جاچ_dir(current->name, device, &new_list, &ret_رستو);
		حرر(current->name);
		حرر(current);
		إذا (ret_رستو)
			اكسر;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		إذا (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			إذا (++level >= EXT2FS_MAX_NESTED_LINKS)
				اكسر;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	أرجع ret_رستو;
}


#جيڪڏهن_وصف_ٿيل DEBUG
صحيح مکيه(صحيح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحيح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	إذا ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		ٻاهر_نڪرو(1);
	}
	إذا (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			ٻاهر_نڪرو(1);
		}
	} وإلا {
		major = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			ٻاهر_نڪرو(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			ٻاهر_نڪرو(1);
		}
		device = makedev(major, minor);
		لکو("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_ڳوليو_بلاڪ_device(device);
	إذا (devname) {
		لکو("Found device!  %s\n", devname);
		حرر(devname);
	} وإلا {
		لکو("Couldn't find device.\n");
	}
	أرجع 0;
}

#جيڪڏهن_پورو
