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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <dirent.h>
#якщо HAVE_ERRNO_H
#include <errno.h>
#endif
#якщо HAVE_SYS_MKDEV_H
#include <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#include <sys/sysmacros.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

struct dir_list {
	char	*name;
	struct dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
static void add_to_dirlist(const char *name, struct dir_list **list)
{
	struct dir_list *dp;

	dp = malloc(sizeof(struct dir_list));
	якщо (!dp)
		повернути;
	dp->name = malloc(strlen(name)+1);
	якщо (!dp->name) {
		free(dp);
		повернути;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
static void free_dirlist(struct dir_list **list)
{
	struct dir_list *dp, *next;

	для (dp = *list; dp; dp = next) {
		next = dp->next;
		free(dp->name);
		free(dp);
	}
	*list = 0;
}

static ціле scan_dir(char *dirname, dev_t device, struct dir_list **list,
		    char **ret_path)
{
	DIR	*dir;
	struct dirent *dp;
	char	path[1024], *cp;
	ціле	dirlen;
	struct stat st;

	dirlen = strlen(dirname);
	якщо ((dir = opendir(dirname)) == NULL)
		повернути errno;
	dp = readdir(dir);
	поки (dp) {
		якщо (dirlen + strlen(dp->d_name) + 2 >= sizeof(path))
			goto skip_to_next;
		якщо (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			goto skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		якщо (stat(path, &st) < 0)
			goto skip_to_next;
		якщо (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		якщо (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			якщо (!cp) {
				closedir(dir);
				повернути ENOMEM;
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
	повернути 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
char *ext2fs_find_block_device(dev_t device)
{
	struct dir_list *list = 0, *new_list = 0;
	struct dir_list *current;
	char	*ret_path = 0;
	ціле    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	поки (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		free(current->name);
		free(current);
		якщо (ret_path)
			break;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		якщо (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			якщо (++level >= EXT2FS_MAX_NESTED_LINKS)
				break;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	повернути ret_path;
}


#ifdef DEBUG
ціле main(ціле argc, char** argv)
{
	char	*devname, *tmp;
	ціле	major, minor;
	dev_t	device;
	const char *errmsg = "Couldn't parse %s: %s\n";

	якщо ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	якщо (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		якщо (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} інакше {
		major = strtoul(argv[1], &tmp, 0);
		якщо (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		якщо (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	якщо (devname) {
		printf("Found device!  %s\n", devname);
		free(devname);
	} інакше {
		printf("Couldn't find device.\n");
	}
	повернути 0;
}

#endif
