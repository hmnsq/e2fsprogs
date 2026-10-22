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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#அந்தர்பாவ <dirent.h>
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#எனில் HAVE_SYS_MKDEV_H
#அந்தர்பாவ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#அந்தர்பாவ <sys/sysmacros.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு dir_list {
	எழுத்து	*name;
	கட்டமைப்பு dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
நிலையான வெறுமை add_to_dirlist(மாறிலி எழுத்து *name, கட்டமைப்பு dir_list **list)
{
	கட்டமைப்பு dir_list *dp;

	dp = malloc(அளவு(கட்டமைப்பு dir_list));
	எனில் (!dp)
		திரும்பு;
	dp->name = malloc(strlen(name)+1);
	எனில் (!dp->name) {
		விடுவி(dp);
		திரும்பு;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
நிலையான வெறுமை free_dirlist(கட்டமைப்பு dir_list **list)
{
	கட்டமைப்பு dir_list *dp, *next;

	க்கு (dp = *list; dp; dp = next) {
		next = dp->next;
		விடுவி(dp->name);
		விடுவி(dp);
	}
	*list = 0;
}

நிலையான முழு scan_dir(எழுத்து *dirname, dev_t device, கட்டமைப்பு dir_list **list,
		    எழுத்து **ret_path)
{
	DIR	*dir;
	கட்டமைப்பு dirent *dp;
	எழுத்து	path[1024], *cp;
	முழு	dirlen;
	கட்டமைப்பு stat st;

	dirlen = strlen(dirname);
	எனில் ((dir = opendir(dirname)) == வெற்று)
		திரும்பு errno;
	dp = readdir(dir);
	போது (dp) {
		எனில் (dirlen + strlen(dp->d_name) + 2 >= அளவு(path))
			செல் skip_to_next;
		எனில் (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			செல் skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		எனில் (stat(path, &st) < 0)
			செல் skip_to_next;
		எனில் (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		எனில் (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			எனில் (!cp) {
				closedir(dir);
				திரும்பு ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			செல் success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	திரும்பு 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
எழுத்து *ext2fs_find_block_device(dev_t device)
{
	கட்டமைப்பு dir_list *list = 0, *new_list = 0;
	கட்டமைப்பு dir_list *current;
	எழுத்து	*ret_path = 0;
	முழு    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	போது (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		விடுவி(current->name);
		விடுவி(current);
		எனில் (ret_path)
			நிறுத்து;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		எனில் (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			எனில் (++level >= EXT2FS_MAX_NESTED_LINKS)
				நிறுத்து;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	திரும்பு ret_path;
}


#ifdef DEBUG
முழு முதன்மை(முழு argc, எழுத்து** argv)
{
	எழுத்து	*devname, *tmp;
	முழு	major, minor;
	dev_t	device;
	மாறிலி எழுத்து *errmsg = "Couldn't parse %s: %s\n";

	எனில் ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	எனில் (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		எனில் (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} இல்லையெனில் {
		major = strtoul(argv[1], &tmp, 0);
		எனில் (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		எனில் (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	எனில் (devname) {
		printf("Found device!  %s\n", devname);
		விடுவி(devname);
	} இல்லையெனில் {
		printf("Couldn't find device.\n");
	}
	திரும்பு 0;
}

#endif
