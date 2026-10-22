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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#অন্তর্ভাৱ <dirent.h>
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#যদি HAVE_SYS_MKDEV_H
#অন্তর্ভাৱ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#অন্তর্ভাৱ <sys/sysmacros.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

সংরচনা dir_list {
	ৱর্ণ	*name;
	সংরচনা dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
স্থির শূন্য add_to_dirlist(ধ্রুৱ ৱর্ণ *name, সংরচনা dir_list **list)
{
	সংরচনা dir_list *dp;

	dp = malloc(পরিমাণ(সংরচনা dir_list));
	যদি (!dp)
		ঘূৰাই;
	dp->name = malloc(strlen(name)+1);
	যদি (!dp->name) {
		মুক্ত(dp);
		ঘূৰাই;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
স্থির শূন্য free_dirlist(সংরচনা dir_list **list)
{
	সংরচনা dir_list *dp, *next;

	বাবে (dp = *list; dp; dp = next) {
		next = dp->next;
		মুক্ত(dp->name);
		মুক্ত(dp);
	}
	*list = 0;
}

স্থির পূর্ণাঙ্ক scan_dir(ৱর্ণ *dirname, dev_t device, সংরচনা dir_list **list,
		    ৱর্ণ **ret_path)
{
	DIR	*dir;
	সংরচনা dirent *dp;
	ৱর্ণ	path[1024], *cp;
	পূর্ণাঙ্ক	dirlen;
	সংরচনা stat st;

	dirlen = strlen(dirname);
	যদি ((dir = opendir(dirname)) == রিক্ত)
		ঘূৰাই errno;
	dp = readdir(dir);
	যেতিয়ালৈ (dp) {
		যদি (dirlen + strlen(dp->d_name) + 2 >= পরিমাণ(path))
			গচ্ছ skip_to_next;
		যদি (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			গচ্ছ skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		যদি (stat(path, &st) < 0)
			গচ্ছ skip_to_next;
		যদি (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		যদি (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			যদি (!cp) {
				closedir(dir);
				ঘূৰাই ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			গচ্ছ success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ঘূৰাই 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
ৱর্ণ *ext2fs_find_block_device(dev_t device)
{
	সংরচনা dir_list *list = 0, *new_list = 0;
	সংরচনা dir_list *current;
	ৱর্ণ	*ret_path = 0;
	পূর্ণাঙ্ক    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	যেতিয়ালৈ (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		মুক্ত(current->name);
		মুক্ত(current);
		যদি (ret_path)
			ভঙ্গ;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		যদি (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			যদি (++level >= EXT2FS_MAX_NESTED_LINKS)
				ভঙ্গ;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ঘূৰাই ret_path;
}


#ifdef DEBUG
পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ** argv)
{
	ৱর্ণ	*devname, *tmp;
	পূর্ণাঙ্ক	major, minor;
	dev_t	device;
	ধ্রুৱ ৱর্ণ *errmsg = "Couldn't parse %s: %s\n";

	যদি ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	যদি (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		যদি (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} অন্যথা {
		major = strtoul(argv[1], &tmp, 0);
		যদি (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		যদি (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	যদি (devname) {
		printf("Found device!  %s\n", devname);
		মুক্ত(devname);
	} অন্যথা {
		printf("Couldn't find device.\n");
	}
	ঘূৰাই 0;
}

#endif
