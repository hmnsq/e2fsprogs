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
#پایان_اگر
#ضمن <stdlib.h>
#ضمن <string.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#ضمن <dirent.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#إذا HAVE_SYS_MKDEV_H
#ضمن <sys/mkdev.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية dir_list {
	حرف	*name;
	بنية dir_list *بعدی;
};

/*
 * This function adds an entry to the directory list
 */
ساكن فراغ جمع_کن_مقصد_dirlist(ثابت حرف *name, بنية dir_list **list)
{
	بنية dir_list *dp;

	dp = حافظه_بده(حجم(بنية dir_list));
	إذا (!dp)
		أرجع;
	dp->name = حافظه_بده(طول_رشته(name)+1);
	إذا (!dp->name) {
		حرر(dp);
		أرجع;
	}
	strcpy(dp->name, name);
	dp->بعدی = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساكن فراغ free_dirlist(بنية dir_list **list)
{
	بنية dir_list *dp, *بعدی;

	لكل (dp = *list; dp; dp = بعدی) {
		بعدی = dp->بعدی;
		حرر(dp->name);
		حرر(dp);
	}
	*list = 0;
}

ساكن صحيح پیمایش_dir(حرف *dirname, dev_t device, بنية dir_list **list,
		    حرف **ret_مسیر)
{
	DIR	*dir;
	بنية dirent *dp;
	حرف	مسیر[1024], *cp;
	صحيح	dirlen;
	بنية stat st;

	dirlen = طول_رشته(dirname);
	إذا ((dir = opendir(dirname)) == عدم)
		أرجع errno;
	dp = readdir(dir);
	طالما (dp) {
		إذا (dirlen + طول_رشته(dp->d_name) + 2 >= حجم(مسیر))
			اذهب skip_مقصد_بعدی;
		إذا (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			اذهب skip_مقصد_بعدی;
		sprintf(مسیر, "%s/%s", dirname, dp->d_name);
		إذا (stat(مسیر, &st) < 0)
			اذهب skip_مقصد_بعدی;
		إذا (S_ISDIR(st.st_نما))
			جمع_کن_مقصد_dirlist(مسیر, list);
		إذا (ext2fsP_is_disk_device(st.st_نما) &&
		    st.st_rdev == device) {
			cp = حافظه_بده(طول_رشته(مسیر)+1);
			إذا (!cp) {
				closedir(dir);
				أرجع ENOMEM;
			}
			strcpy(cp, مسیر);
			*ret_مسیر = cp;
			اذهب success;
		}
	skip_مقصد_بعدی:
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
حرف *ext2fs_پیدا_کن_بلوک_device(dev_t device)
{
	بنية dir_list *list = 0, *new_list = 0;
	بنية dir_list *current;
	حرف	*ret_مسیر = 0;
	صحيح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	جمع_کن_مقصد_dirlist("/devices", &list);
	جمع_کن_مقصد_dirlist("/devfs", &list);
	جمع_کن_مقصد_dirlist("/dev", &list);

	طالما (list) {
		current = list;
		list = list->بعدی;
#اگر_تعریف_شده DEBUG
		چاپ_کن("Scanning directory %s\n", current->name);
#پایان_اگر
		پیمایش_dir(current->name, device, &new_list, &ret_مسیر);
		حرر(current->name);
		حرر(current);
		إذا (ret_مسیر)
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
	أرجع ret_مسیر;
}


#اگر_تعریف_شده DEBUG
صحيح اصلی(صحيح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحيح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	إذا ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		خروج(1);
	}
	إذا (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			خروج(1);
		}
	} وإلا {
		major = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			خروج(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			خروج(1);
		}
		device = makedev(major, minor);
		چاپ_کن("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_پیدا_کن_بلوک_device(device);
	إذا (devname) {
		چاپ_کن("Found device!  %s\n", devname);
		حرر(devname);
	} وإلا {
		چاپ_کن("Couldn't find device.\n");
	}
	أرجع 0;
}

#پایان_اگر
