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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಯದಿ HAVE_SYS_MKDEV_H
#ಅನ್ತರ್ಭಾವ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ dir_list {
	ವರ್ಣ	*name;
	ಸಂರಚನಾ dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
ಸ್ಥಿರ ಶೂನ್ಯ add_to_dirlist(ಧ್ರುವ ವರ್ಣ *name, ಸಂರಚನಾ dir_list **list)
{
	ಸಂರಚನಾ dir_list *dp;

	dp = malloc(ಪರಿಮಾಣ(ಸಂರಚನಾ dir_list));
	ಯದಿ (!dp)
		ಪ್ರತ್ಯಾಗಮ;
	dp->name = malloc(strlen(name)+1);
	ಯದಿ (!dp->name) {
		ಮುಕ್ತ(dp);
		ಪ್ರತ್ಯಾಗಮ;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ಸ್ಥಿರ ಶೂನ್ಯ free_dirlist(ಸಂರಚನಾ dir_list **list)
{
	ಸಂರಚನಾ dir_list *dp, *next;

	ಪ್ರತಿ (dp = *list; dp; dp = next) {
		next = dp->next;
		ಮುಕ್ತ(dp->name);
		ಮುಕ್ತ(dp);
	}
	*list = 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ scan_dir(ವರ್ಣ *dirname, dev_t device, ಸಂರಚನಾ dir_list **list,
		    ವರ್ಣ **ret_path)
{
	DIR	*dir;
	ಸಂರಚನಾ dirent *dp;
	ವರ್ಣ	path[1024], *cp;
	ಪೂರ್ಣಾಙ್ಕ	dirlen;
	ಸಂರಚನಾ stat st;

	dirlen = strlen(dirname);
	ಯದಿ ((dir = opendir(dirname)) == ರಿಕ್ತ)
		ಪ್ರತ್ಯಾಗಮ errno;
	dp = readdir(dir);
	ಯಾವತ್ (dp) {
		ಯದಿ (dirlen + strlen(dp->d_name) + 2 >= ಪರಿಮಾಣ(path))
			ಗಚ್ಛ skip_to_next;
		ಯದಿ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			ಗಚ್ಛ skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		ಯದಿ (stat(path, &st) < 0)
			ಗಚ್ಛ skip_to_next;
		ಯದಿ (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		ಯದಿ (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			ಯದಿ (!cp) {
				closedir(dir);
				ಪ್ರತ್ಯಾಗಮ ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			ಗಚ್ಛ success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
ವರ್ಣ *ext2fs_find_block_device(dev_t device)
{
	ಸಂರಚನಾ dir_list *list = 0, *new_list = 0;
	ಸಂರಚನಾ dir_list *current;
	ವರ್ಣ	*ret_path = 0;
	ಪೂರ್ಣಾಙ್ಕ    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	ಯಾವತ್ (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		ಮುಕ್ತ(current->name);
		ಮುಕ್ತ(current);
		ಯದಿ (ret_path)
			ಭಙ್ಗ;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ಯದಿ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ಯದಿ (++level >= EXT2FS_MAX_NESTED_LINKS)
				ಭಙ್ಗ;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ಪ್ರತ್ಯಾಗಮ ret_path;
}


#ifdef DEBUG
ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ** argv)
{
	ವರ್ಣ	*devname, *tmp;
	ಪೂರ್ಣಾಙ್ಕ	major, minor;
	dev_t	device;
	ಧ್ರುವ ವರ್ಣ *errmsg = "Couldn't parse %s: %s\n";

	ಯದಿ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	ಯದಿ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ಯದಿ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} ಅನ್ಯಥಾ {
		major = strtoul(argv[1], &tmp, 0);
		ಯದಿ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ಯದಿ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	ಯದಿ (devname) {
		printf("Found device!  %s\n", devname);
		ಮುಕ್ತ(devname);
	} ಅನ್ಯಥಾ {
		printf("Couldn't find device.\n");
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

#endif
