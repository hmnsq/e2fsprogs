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
#جے_انت
#ضمن <stdlib.h>
#ضمن <string.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#ضمن <dirent.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#إذا HAVE_SYS_MKDEV_H
#ضمن <sys/mkdev.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية dir_list {
	حرف	*name;
	بنية dir_list *اگلا;
};

/*
 * This function adds an entry to the directory list
 */
ساكن فراغ جوڑو_منزل_dirlist(ثابت حرف *name, بنية dir_list **list)
{
	بنية dir_list *dp;

	dp = حافظہ_دیو(حجم(بنية dir_list));
	إذا (!dp)
		أرجع;
	dp->name = حافظہ_دیو(لڑی_لمبائی(name)+1);
	إذا (!dp->name) {
		حرر(dp);
		أرجع;
	}
	strcpy(dp->name, name);
	dp->اگلا = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساكن فراغ free_dirlist(بنية dir_list **list)
{
	بنية dir_list *dp, *اگلا;

	لكل (dp = *list; dp; dp = اگلا) {
		اگلا = dp->اگلا;
		حرر(dp->name);
		حرر(dp);
	}
	*list = 0;
}

ساكن صحيح جانچ_dir(حرف *dirname, dev_t device, بنية dir_list **list,
		    حرف **ret_رستہ)
{
	DIR	*dir;
	بنية dirent *dp;
	حرف	رستہ[1024], *cp;
	صحيح	dirlen;
	بنية stat st;

	dirlen = لڑی_لمبائی(dirname);
	إذا ((dir = opendir(dirname)) == عدم)
		أرجع errno;
	dp = readdir(dir);
	طالما (dp) {
		إذا (dirlen + لڑی_لمبائی(dp->d_name) + 2 >= حجم(رستہ))
			اذهب skip_منزل_اگلا;
		إذا (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			اذهب skip_منزل_اگلا;
		sprintf(رستہ, "%s/%s", dirname, dp->d_name);
		إذا (stat(رستہ, &st) < 0)
			اذهب skip_منزل_اگلا;
		إذا (S_ISDIR(st.st_منوال))
			جوڑو_منزل_dirlist(رستہ, list);
		إذا (ext2fsP_is_disk_device(st.st_منوال) &&
		    st.st_rdev == device) {
			cp = حافظہ_دیو(لڑی_لمبائی(رستہ)+1);
			إذا (!cp) {
				closedir(dir);
				أرجع ENOMEM;
			}
			strcpy(cp, رستہ);
			*ret_رستہ = cp;
			اذهب success;
		}
	skip_منزل_اگلا:
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
حرف *ext2fs_لبھو_بلاک_device(dev_t device)
{
	بنية dir_list *list = 0, *new_list = 0;
	بنية dir_list *current;
	حرف	*ret_رستہ = 0;
	صحيح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	جوڑو_منزل_dirlist("/devices", &list);
	جوڑو_منزل_dirlist("/devfs", &list);
	جوڑو_منزل_dirlist("/dev", &list);

	طالما (list) {
		current = list;
		list = list->اگلا;
#جے_تعریف_شدہ DEBUG
		لکھو("Scanning directory %s\n", current->name);
#جے_انت
		جانچ_dir(current->name, device, &new_list, &ret_رستہ);
		حرر(current->name);
		حرر(current);
		إذا (ret_رستہ)
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
	أرجع ret_رستہ;
}


#جے_تعریف_شدہ DEBUG
صحيح مکھ(صحيح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحيح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	إذا ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		باہر_نکلو(1);
	}
	إذا (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			باہر_نکلو(1);
		}
	} وإلا {
		major = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			باہر_نکلو(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			باہر_نکلو(1);
		}
		device = makedev(major, minor);
		لکھو("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_لبھو_بلاک_device(device);
	إذا (devname) {
		لکھو("Found device!  %s\n", devname);
		حرر(devname);
	} وإلا {
		لکھو("Couldn't find device.\n");
	}
	أرجع 0;
}

#جے_انت
