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
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>
#include <string.h>
#만약 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#만약 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <dirent.h>
#만약 HAVE_ERRNO_H
#include <errno.h>
#endif
#만약 HAVE_SYS_MKDEV_H
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
	만약 (!dp)
		반환;
	dp->name = malloc(strlen(name)+1);
	만약 (!dp->name) {
		free(dp);
		반환;
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

	위해 (dp = *list; dp; dp = next) {
		next = dp->next;
		free(dp->name);
		free(dp);
	}
	*list = 0;
}

static 정수 scan_dir(char *dirname, dev_t device, struct dir_list **list,
		    char **ret_path)
{
	DIR	*dir;
	struct dirent *dp;
	char	path[1024], *cp;
	정수	dirlen;
	struct stat st;

	dirlen = strlen(dirname);
	만약 ((dir = opendir(dirname)) == NULL)
		반환 errno;
	dp = readdir(dir);
	동안 (dp) {
		만약 (dirlen + strlen(dp->d_name) + 2 >= sizeof(path))
			goto skip_to_next;
		만약 (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			goto skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		만약 (stat(path, &st) < 0)
			goto skip_to_next;
		만약 (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		만약 (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			만약 (!cp) {
				closedir(dir);
				반환 ENOMEM;
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
	반환 0;
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
	정수    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	동안 (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		free(current->name);
		free(current);
		만약 (ret_path)
			break;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		만약 (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			만약 (++level >= EXT2FS_MAX_NESTED_LINKS)
				break;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	반환 ret_path;
}


#ifdef DEBUG
정수 main(정수 argc, char** argv)
{
	char	*devname, *tmp;
	정수	major, minor;
	dev_t	device;
	const char *errmsg = "Couldn't parse %s: %s\n";

	만약 ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	만약 (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		만약 (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} 아니면 {
		major = strtoul(argv[1], &tmp, 0);
		만약 (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		만약 (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	만약 (devname) {
		printf("Found device!  %s\n", devname);
		free(devname);
	} 아니면 {
		printf("Couldn't find device.\n");
	}
	반환 0;
}

#endif
