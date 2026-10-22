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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <stdlib.h>
#شامل <string.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#شامل <dirent.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#اگر HAVE_SYS_MKDEV_H
#شامل <sys/mkdev.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_SYSMACROS_H
#شامل <sys/sysmacros.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساخت dir_list {
	حرف	*name;
	ساخت dir_list *اگلا;
};

/*
 * This function adds an entry to the directory list
 */
ساکن خالی جوڑو_مقصد_dirlist(ثابت حرف *name, ساخت dir_list **list)
{
	ساخت dir_list *dp;

	dp = حافظہ_دو(حجم(ساخت dir_list));
	اگر (!dp)
		واپس;
	dp->name = حافظہ_دو(لڑی_لمبائی(name)+1);
	اگر (!dp->name) {
		آزاد(dp);
		واپس;
	}
	strcpy(dp->name, name);
	dp->اگلا = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساکن خالی free_dirlist(ساخت dir_list **list)
{
	ساخت dir_list *dp, *اگلا;

	برائے (dp = *list; dp; dp = اگلا) {
		اگلا = dp->اگلا;
		آزاد(dp->name);
		آزاد(dp);
	}
	*list = 0;
}

ساکن صحیح جائزہ_dir(حرف *dirname, dev_t device, ساخت dir_list **list,
		    حرف **ret_راستہ)
{
	DIR	*dir;
	ساخت dirent *dp;
	حرف	راستہ[1024], *cp;
	صحیح	dirlen;
	ساخت stat st;

	dirlen = لڑی_لمبائی(dirname);
	اگر ((dir = opendir(dirname)) == تہی)
		واپس errno;
	dp = readdir(dir);
	جب (dp) {
		اگر (dirlen + لڑی_لمبائی(dp->d_name) + 2 >= حجم(راستہ))
			جاؤ skip_مقصد_اگلا;
		اگر (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			جاؤ skip_مقصد_اگلا;
		sprintf(راستہ, "%s/%s", dirname, dp->d_name);
		اگر (stat(راستہ, &st) < 0)
			جاؤ skip_مقصد_اگلا;
		اگر (S_ISDIR(st.st_منوال))
			جوڑو_مقصد_dirlist(راستہ, list);
		اگر (ext2fsP_is_disk_device(st.st_منوال) &&
		    st.st_rdev == device) {
			cp = حافظہ_دو(لڑی_لمبائی(راستہ)+1);
			اگر (!cp) {
				closedir(dir);
				واپس ENOMEM;
			}
			strcpy(cp, راستہ);
			*ret_راستہ = cp;
			جاؤ success;
		}
	skip_مقصد_اگلا:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	واپس 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
حرف *ext2fs_ڈھونڈو_بلاک_device(dev_t device)
{
	ساخت dir_list *list = 0, *new_list = 0;
	ساخت dir_list *current;
	حرف	*ret_راستہ = 0;
	صحیح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	جوڑو_مقصد_dirlist("/devices", &list);
	جوڑو_مقصد_dirlist("/devfs", &list);
	جوڑو_مقصد_dirlist("/dev", &list);

	جب (list) {
		current = list;
		list = list->اگلا;
#اگر_تعریف_شدہ DEBUG
		لکھو("Scanning directory %s\n", current->name);
#اگر_ختم
		جائزہ_dir(current->name, device, &new_list, &ret_راستہ);
		آزاد(current->name);
		آزاد(current);
		اگر (ret_راستہ)
			توڑو;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		اگر (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			اگر (++level >= EXT2FS_MAX_NESTED_LINKS)
				توڑو;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	واپس ret_راستہ;
}


#اگر_تعریف_شدہ DEBUG
صحیح مین(صحیح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحیح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	اگر ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		باہر_نکلو(1);
	}
	اگر (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			باہر_نکلو(1);
		}
	} ورنہ {
		major = strtoul(argv[1], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			باہر_نکلو(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			باہر_نکلو(1);
		}
		device = makedev(major, minor);
		لکھو("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_ڈھونڈو_بلاک_device(device);
	اگر (devname) {
		لکھو("Found device!  %s\n", devname);
		آزاد(devname);
	} ورنہ {
		لکھو("Couldn't find device.\n");
	}
	واپس 0;
}

#اگر_ختم
