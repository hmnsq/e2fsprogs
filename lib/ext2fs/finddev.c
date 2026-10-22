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
#د_که_پای
#ضمن <stdlib.h>
#ضمن <string.h>
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#ضمن <dirent.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#إذا HAVE_SYS_MKDEV_H
#ضمن <sys/mkdev.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية dir_list {
	حرف	*name;
	بنية dir_list *بل;
};

/*
 * This function adds an entry to the directory list
 */
ساكن فراغ جمع_کړه_منزل_dirlist(ثابت حرف *name, بنية dir_list **list)
{
	بنية dir_list *dp;

	dp = حافظه_ورکړه(حجم(بنية dir_list));
	إذا (!dp)
		أرجع;
	dp->name = حافظه_ورکړه(مزي_اوږدوالی(name)+1);
	إذا (!dp->name) {
		حرر(dp);
		أرجع;
	}
	strcpy(dp->name, name);
	dp->بل = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساكن فراغ free_dirlist(بنية dir_list **list)
{
	بنية dir_list *dp, *بل;

	لكل (dp = *list; dp; dp = بل) {
		بل = dp->بل;
		حرر(dp->name);
		حرر(dp);
	}
	*list = 0;
}

ساكن صحيح لټون_dir(حرف *dirname, dev_t device, بنية dir_list **list,
		    حرف **ret_لار)
{
	DIR	*dir;
	بنية dirent *dp;
	حرف	لار[1024], *cp;
	صحيح	dirlen;
	بنية stat st;

	dirlen = مزي_اوږدوالی(dirname);
	إذا ((dir = opendir(dirname)) == عدم)
		أرجع errno;
	dp = readdir(dir);
	طالما (dp) {
		إذا (dirlen + مزي_اوږدوالی(dp->d_name) + 2 >= حجم(لار))
			اذهب skip_منزل_بل;
		إذا (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			اذهب skip_منزل_بل;
		sprintf(لار, "%s/%s", dirname, dp->d_name);
		إذا (stat(لار, &st) < 0)
			اذهب skip_منزل_بل;
		إذا (S_ISDIR(st.st_ډېرپېښ))
			جمع_کړه_منزل_dirlist(لار, list);
		إذا (ext2fsP_is_disk_device(st.st_ډېرپېښ) &&
		    st.st_rdev == device) {
			cp = حافظه_ورکړه(مزي_اوږدوالی(لار)+1);
			إذا (!cp) {
				closedir(dir);
				أرجع ENOMEM;
			}
			strcpy(cp, لار);
			*ret_لار = cp;
			اذهب success;
		}
	skip_منزل_بل:
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
حرف *ext2fs_پیدا_کړه_بلاک_device(dev_t device)
{
	بنية dir_list *list = 0, *new_list = 0;
	بنية dir_list *current;
	حرف	*ret_لار = 0;
	صحيح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	جمع_کړه_منزل_dirlist("/devices", &list);
	جمع_کړه_منزل_dirlist("/devfs", &list);
	جمع_کړه_منزل_dirlist("/dev", &list);

	طالما (list) {
		current = list;
		list = list->بل;
#که_تعریف_شوی DEBUG
		چاپ_کړه("Scanning directory %s\n", current->name);
#د_که_پای
		لټون_dir(current->name, device, &new_list, &ret_لار);
		حرر(current->name);
		حرر(current);
		إذا (ret_لار)
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
	أرجع ret_لار;
}


#که_تعریف_شوی DEBUG
صحيح اصلي(صحيح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحيح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	إذا ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		وتل(1);
	}
	إذا (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			وتل(1);
		}
	} وإلا {
		major = strtoul(argv[1], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			وتل(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		إذا (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			وتل(1);
		}
		device = makedev(major, minor);
		چاپ_کړه("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_پیدا_کړه_بلاک_device(device);
	إذا (devname) {
		چاپ_کړه("Found device!  %s\n", devname);
		حرر(devname);
	} وإلا {
		چاپ_کړه("Couldn't find device.\n");
	}
	أرجع 0;
}

#د_که_پای
