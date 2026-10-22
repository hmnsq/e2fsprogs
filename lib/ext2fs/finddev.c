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
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#ပေႃး HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ပေႃး HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <dirent.h>
#ပေႃး HAVE_ERRNO_H
#include <errno.h>
#endif
#ပေႃး HAVE_SYS_MKDEV_H
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
	ပေႃး (!dp)
		ပွၵ်ႈ;
	dp->name = malloc(strlen(name)+1);
	ပေႃး (!dp->name) {
		free(dp);
		ပွၵ်ႈ;
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

	for (dp = *list; dp; dp = next) {
		next = dp->next;
		free(dp->name);
		free(dp);
	}
	*list = 0;
}

static puur_naa_nka scan_dir(var_na *dirname, dev_t device, sa_nracanaa dir_list **list,
		    var_na **ret_path)
{
	DIR	*dir;
	sa_nracanaa dirent *dp;
	var_na	path[1024], *cp;
	puur_naa_nka	dirlen;
	sa_nracanaa stat st;

	dirlen = strlen(dirname);
	ပေႃး ((dir = opendir(dirname)) == NULL)
		ပွၵ်ႈ errno;
	dp = readdir(dir);
	ၶၢဝ်း (dp) {
		ပေႃး (dirlen + strlen(dp->d_name) + 2 >= parimaa_na(path))
			goto skip_to_next;
		ပေႃး (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			goto skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		ပေႃး (stat(path, &st) < 0)
			goto skip_to_next;
		ပေႃး (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		ပေႃး (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			ပေႃး (!cp) {
				closedir(dir);
				ပွၵ်ႈ ENOMEM;
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
	ပွၵ်ႈ 0;
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
	puur_naa_nka    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	ၶၢဝ်း (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		free(current->name);
		free(current);
		ပေႃး (ret_path)
			bha_nga;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ပေႃး (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ပေႃး (++level >= EXT2FS_MAX_NESTED_LINKS)
				bha_nga;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ပွၵ်ႈ ret_path;
}


#ifdef DEBUG
puur_naa_nka main(puur_naa_nka argc, var_na** argv)
{
	var_na	*devname, *tmp;
	puur_naa_nka	major, minor;
	dev_t	device;
	const var_na *errmsg = "Couldn't parse %s: %s\n";

	ပေႃး ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	ပေႃး (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ပေႃး (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} ပေႃးဢမ်ႇ {
		major = strtoul(argv[1], &tmp, 0);
		ပေႃး (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ပေႃး (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	ပေႃး (devname) {
		printf("Found device!  %s\n", devname);
		free(devname);
	} ပေႃးဢမ်ႇ {
		printf("Couldn't find device.\n");
	}
	ပွၵ်ႈ 0;
}

#endif
