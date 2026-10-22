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
#endif
#شامل <stdlib.h>
#شامل <string.h>
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#شامل <dirent.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#اگر HAVE_SYS_MKDEV_H
#شامل <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#شامل <sys/sysmacros.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساختار dir_list {
	نویسه	*name;
	ساختار dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
ایستا تهی add_to_dirlist(ثابت نویسه *name, ساختار dir_list **list)
{
	ساختار dir_list *dp;

	dp = malloc(اندازه(ساختار dir_list));
	اگر (!dp)
		بازگشت;
	dp->name = malloc(strlen(name)+1);
	اگر (!dp->name) {
		آزاد(dp);
		بازگشت;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ایستا تهی free_dirlist(ساختار dir_list **list)
{
	ساختار dir_list *dp, *next;

	برای (dp = *list; dp; dp = next) {
		next = dp->next;
		آزاد(dp->name);
		آزاد(dp);
	}
	*list = 0;
}

ایستا صحیح scan_dir(نویسه *dirname, dev_t device, ساختار dir_list **list,
		    نویسه **ret_path)
{
	DIR	*dir;
	ساختار dirent *dp;
	نویسه	path[1024], *cp;
	صحیح	dirlen;
	ساختار stat st;

	dirlen = strlen(dirname);
	اگر ((dir = opendir(dirname)) == پوچ)
		بازگشت errno;
	dp = readdir(dir);
	وقتی (dp) {
		اگر (dirlen + strlen(dp->d_name) + 2 >= اندازه(path))
			برو skip_to_next;
		اگر (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			برو skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		اگر (stat(path, &st) < 0)
			برو skip_to_next;
		اگر (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		اگر (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			اگر (!cp) {
				closedir(dir);
				بازگشت ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			برو success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	بازگشت 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
نویسه *ext2fs_find_block_device(dev_t device)
{
	ساختار dir_list *list = 0, *new_list = 0;
	ساختار dir_list *current;
	نویسه	*ret_path = 0;
	صحیح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	وقتی (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		آزاد(current->name);
		آزاد(current);
		اگر (ret_path)
			بشکن;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		اگر (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			اگر (++level >= EXT2FS_MAX_NESTED_LINKS)
				بشکن;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	بازگشت ret_path;
}


#ifdef DEBUG
صحیح اصلی(صحیح argc, نویسه** argv)
{
	نویسه	*devname, *tmp;
	صحیح	major, minor;
	dev_t	device;
	ثابت نویسه *errmsg = "Couldn't parse %s: %s\n";

	اگر ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	اگر (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} وگرنه {
		major = strtoul(argv[1], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		اگر (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	اگر (devname) {
		printf("Found device!  %s\n", devname);
		آزاد(devname);
	} وگرنه {
		printf("Couldn't find device.\n");
	}
	بازگشت 0;
}

#endif
