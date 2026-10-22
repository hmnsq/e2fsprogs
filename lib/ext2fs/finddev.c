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

#include "config.h"
#include <stdio.h>
#include <string.h>
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#အကယ်၍ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <dirent.h>
#အကယ်၍ HAVE_ERRNO_H
#include <errno.h>
#endif
#အကယ်၍ HAVE_SYS_MKDEV_H
#include <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#include <sys/sysmacros.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

sa_nracanaa dir_list {
	var_na	*name;
	sa_nracanaa dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
static void add_to_dirlist(const var_na *name, sa_nracanaa dir_list **list)
{
	sa_nracanaa dir_list *dp;

	dp = malloc(parimaa_na(sa_nracanaa dir_list));
	အကယ်၍ (!dp)
		ပြန်ပေး;
	dp->name = malloc(strlen(name)+1);
	အကယ်၍ (!dp->name) {
		free(dp);
		ပြန်ပေး;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
static void free_dirlist(sa_nracanaa dir_list **list)
{
	sa_nracanaa dir_list *dp, *next;

	အတွက် (dp = *list; dp; dp = next) {
		next = dp->next;
		free(dp->name);
		free(dp);
	}
	*list = 0;
}

static ကိန်းပြည့် scan_dir(var_na *dirname, dev_t device, sa_nracanaa dir_list **list,
		    var_na **ret_path)
{
	DIR	*dir;
	sa_nracanaa dirent *dp;
	var_na	path[1024], *cp;
	ကိန်းပြည့်	dirlen;
	sa_nracanaa stat st;

	dirlen = strlen(dirname);
	အကယ်၍ ((dir = opendir(dirname)) == NULL)
		ပြန်ပေး errno;
	dp = readdir(dir);
	နေစဉ် (dp) {
		အကယ်၍ (dirlen + strlen(dp->d_name) + 2 >= parimaa_na(path))
			goto skip_to_next;
		အကယ်၍ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			goto skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		အကယ်၍ (stat(path, &st) < 0)
			goto skip_to_next;
		အကယ်၍ (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		အကယ်၍ (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			အကယ်၍ (!cp) {
				closedir(dir);
				ပြန်ပေး ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			goto success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ပြန်ပေး 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
var_na *ext2fs_find_block_device(dev_t device)
{
	sa_nracanaa dir_list *list = 0, *new_list = 0;
	sa_nracanaa dir_list *current;
	var_na	*ret_path = 0;
	ကိန်းပြည့်    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	နေစဉ် (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		free(current->name);
		free(current);
		အကယ်၍ (ret_path)
			bha_nga;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		အကယ်၍ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			အကယ်၍ (++level >= EXT2FS_MAX_NESTED_LINKS)
				bha_nga;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ပြန်ပေး ret_path;
}


#ifdef DEBUG
ကိန်းပြည့် main(ကိန်းပြည့် argc, var_na** argv)
{
	var_na	*devname, *tmp;
	ကိန်းပြည့်	major, minor;
	dev_t	device;
	const var_na *errmsg = "Couldn't parse %s: %s\n";

	အကယ်၍ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	အကယ်၍ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		အကယ်၍ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} မဟုတ်ပါက {
		major = strtoul(argv[1], &tmp, 0);
		အကယ်၍ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		အကယ်၍ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	အကယ်၍ (devname) {
		printf("Found device!  %s\n", devname);
		free(devname);
	} မဟုတ်ပါက {
		printf("Couldn't find device.\n");
	}
	ပြန်ပေး 0;
}

#endif
