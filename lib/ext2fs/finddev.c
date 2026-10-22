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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#অন্তর্ভাব <dirent.h>
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#যদি HAVE_SYS_MKDEV_H
#অন্তর্ভাব <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#অন্তর্ভাব <sys/sysmacros.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো dir_list {
	অক্ষর	*name;
	কাঠামো dir_list *পরবর্তী;
};

/*
 * This function adds an entry to the directory list
 */
স্থির শূন্য add_to_dirlist(ধ্রুবক অক্ষর *name, কাঠামো dir_list **list)
{
	কাঠামো dir_list *dp;

	dp = malloc(আকার(কাঠামো dir_list));
	যদি (!dp)
		ফেরত;
	dp->name = malloc(strlen(name)+1);
	যদি (!dp->name) {
		মুক্ত(dp);
		ফেরত;
	}
	strcpy(dp->name, name);
	dp->পরবর্তী = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
স্থির শূন্য free_dirlist(কাঠামো dir_list **list)
{
	কাঠামো dir_list *dp, *পরবর্তী;

	জন্য (dp = *list; dp; dp = পরবর্তী) {
		পরবর্তী = dp->পরবর্তী;
		মুক্ত(dp->name);
		মুক্ত(dp);
	}
	*list = 0;
}

স্থির পূর্ণসংখ্যা scan_dir(অক্ষর *dirname, dev_t device, কাঠামো dir_list **list,
		    অক্ষর **ret_path)
{
	DIR	*dir;
	কাঠামো dirent *dp;
	অক্ষর	path[1024], *cp;
	পূর্ণসংখ্যা	dirlen;
	কাঠামো stat st;

	dirlen = strlen(dirname);
	যদি ((dir = opendir(dirname)) == ফাঁকা)
		ফেরত errno;
	dp = readdir(dir);
	যখন (dp) {
		যদি (dirlen + strlen(dp->d_name) + 2 >= আকার(path))
			যাও skip_to_পরবর্তী;
		যদি (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			যাও skip_to_পরবর্তী;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		যদি (stat(path, &st) < 0)
			যাও skip_to_পরবর্তী;
		যদি (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		যদি (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			যদি (!cp) {
				closedir(dir);
				ফেরত ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			যাও success;
		}
	skip_to_পরবর্তী:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ফেরত 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
অক্ষর *ext2fs_find_block_device(dev_t device)
{
	কাঠামো dir_list *list = 0, *new_list = 0;
	কাঠামো dir_list *current;
	অক্ষর	*ret_path = 0;
	পূর্ণসংখ্যা    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	যখন (list) {
		current = list;
		list = list->পরবর্তী;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		মুক্ত(current->name);
		মুক্ত(current);
		যদি (ret_path)
			বিরতি;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		যদি (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			যদি (++level >= EXT2FS_MAX_NESTED_LINKS)
				বিরতি;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ফেরত ret_path;
}


#ifdef DEBUG
পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর** argv)
{
	অক্ষর	*devname, *tmp;
	পূর্ণসংখ্যা	major, minor;
	dev_t	device;
	ধ্রুবক অক্ষর *errmsg = "Couldn't parse %s: %s\n";

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
	ফেরত 0;
}

#endif
