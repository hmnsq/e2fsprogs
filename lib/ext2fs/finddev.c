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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#അന്തര്ഭാവ <dirent.h>
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#എങ്കിൽ HAVE_SYS_MKDEV_H
#അന്തര്ഭാവ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#അന്തര്ഭാവ <sys/sysmacros.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന dir_list {
	അക്ഷരം	*name;
	ഘടന dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
സ്ഥിതം ശൂന്യം add_to_dirlist(സ്ഥിരം അക്ഷരം *name, ഘടന dir_list **list)
{
	ഘടന dir_list *dp;

	dp = malloc(വലുപ്പം(ഘടന dir_list));
	എങ്കിൽ (!dp)
		തിരികെ;
	dp->name = malloc(strlen(name)+1);
	എങ്കിൽ (!dp->name) {
		സ്വതന്ത്രം(dp);
		തിരികെ;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
സ്ഥിതം ശൂന്യം free_dirlist(ഘടന dir_list **list)
{
	ഘടന dir_list *dp, *next;

	വേണ്ടി (dp = *list; dp; dp = next) {
		next = dp->next;
		സ്വതന്ത്രം(dp->name);
		സ്വതന്ത്രം(dp);
	}
	*list = 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ scan_dir(അക്ഷരം *dirname, dev_t device, ഘടന dir_list **list,
		    അക്ഷരം **ret_path)
{
	DIR	*dir;
	ഘടന dirent *dp;
	അക്ഷരം	path[1024], *cp;
	പൂർണ്ണസംഖ്യ	dirlen;
	ഘടന stat st;

	dirlen = strlen(dirname);
	എങ്കിൽ ((dir = opendir(dirname)) == ശൂന്യ)
		തിരികെ errno;
	dp = readdir(dir);
	ആകുമ്പോൾ (dp) {
		എങ്കിൽ (dirlen + strlen(dp->d_name) + 2 >= വലുപ്പം(path))
			പോകുക skip_to_next;
		എങ്കിൽ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			പോകുക skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		എങ്കിൽ (stat(path, &st) < 0)
			പോകുക skip_to_next;
		എങ്കിൽ (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		എങ്കിൽ (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			എങ്കിൽ (!cp) {
				closedir(dir);
				തിരികെ ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			പോകുക success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	തിരികെ 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
അക്ഷരം *ext2fs_find_block_device(dev_t device)
{
	ഘടന dir_list *list = 0, *new_list = 0;
	ഘടന dir_list *current;
	അക്ഷരം	*ret_path = 0;
	പൂർണ്ണസംഖ്യ    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	ആകുമ്പോൾ (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		സ്വതന്ത്രം(current->name);
		സ്വതന്ത്രം(current);
		എങ്കിൽ (ret_path)
			നിർത്തുക;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		എങ്കിൽ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			എങ്കിൽ (++level >= EXT2FS_MAX_NESTED_LINKS)
				നിർത്തുക;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	തിരികെ ret_path;
}


#ifdef DEBUG
പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം** argv)
{
	അക്ഷരം	*devname, *tmp;
	പൂർണ്ണസംഖ്യ	major, minor;
	dev_t	device;
	സ്ഥിരം അക്ഷരം *errmsg = "Couldn't parse %s: %s\n";

	എങ്കിൽ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	എങ്കിൽ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		എങ്കിൽ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} അല്ലെങ്കിൽ {
		major = strtoul(argv[1], &tmp, 0);
		എങ്കിൽ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		എങ്കിൽ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	എങ്കിൽ (devname) {
		printf("Found device!  %s\n", devname);
		സ്വതന്ത്രം(devname);
	} അല്ലെങ്കിൽ {
		printf("Couldn't find device.\n");
	}
	തിരികെ 0;
}

#endif
