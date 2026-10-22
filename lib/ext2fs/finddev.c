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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଅନ୍ତର୍ଭାଵ <dirent.h>
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଯଦି HAVE_SYS_MKDEV_H
#ଅନ୍ତର୍ଭାଵ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା dir_list {
	ଅକ୍ଷର	*name;
	ସଂରଚନା dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
ସ୍ଥିର ଶୂନ୍ୟ add_to_dirlist(ଅଚଳ ଅକ୍ଷର *name, ସଂରଚନା dir_list **list)
{
	ସଂରଚନା dir_list *dp;

	dp = malloc(ଆକାର(ସଂରଚନା dir_list));
	ଯଦି (!dp)
		ଫେରନ୍ତୁ;
	dp->name = malloc(strlen(name)+1);
	ଯଦି (!dp->name) {
		ମୁକ୍ତ(dp);
		ଫେରନ୍ତୁ;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ସ୍ଥିର ଶୂନ୍ୟ free_dirlist(ସଂରଚନା dir_list **list)
{
	ସଂରଚନା dir_list *dp, *next;

	ପାଇଁ (dp = *list; dp; dp = next) {
		next = dp->next;
		ମୁକ୍ତ(dp->name);
		ମୁକ୍ତ(dp);
	}
	*list = 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା scan_dir(ଅକ୍ଷର *dirname, dev_t device, ସଂରଚନା dir_list **list,
		    ଅକ୍ଷର **ret_path)
{
	DIR	*dir;
	ସଂରଚନା dirent *dp;
	ଅକ୍ଷର	path[1024], *cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	dirlen;
	ସଂରଚନା stat st;

	dirlen = strlen(dirname);
	ଯଦି ((dir = opendir(dirname)) == ଖାଲି)
		ଫେରନ୍ତୁ errno;
	dp = readdir(dir);
	ଯେତେବେଳେ (dp) {
		ଯଦି (dirlen + strlen(dp->d_name) + 2 >= ଆକାର(path))
			ଯାଅ skip_to_next;
		ଯଦି (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			ଯାଅ skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		ଯଦି (stat(path, &st) < 0)
			ଯାଅ skip_to_next;
		ଯଦି (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		ଯଦି (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			ଯଦି (!cp) {
				closedir(dir);
				ଫେରନ୍ତୁ ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			ଯାଅ success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ଫେରନ୍ତୁ 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
ଅକ୍ଷର *ext2fs_find_block_device(dev_t device)
{
	ସଂରଚନା dir_list *list = 0, *new_list = 0;
	ସଂରଚନା dir_list *current;
	ଅକ୍ଷର	*ret_path = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	ଯେତେବେଳେ (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		ମୁକ୍ତ(current->name);
		ମୁକ୍ତ(current);
		ଯଦି (ret_path)
			ଭାଙ୍ଗ;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ଯଦି (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ଯଦି (++level >= EXT2FS_MAX_NESTED_LINKS)
				ଭାଙ୍ଗ;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ଫେରନ୍ତୁ ret_path;
}


#ifdef DEBUG
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର** argv)
{
	ଅକ୍ଷର	*devname, *tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	major, minor;
	dev_t	device;
	ଅଚଳ ଅକ୍ଷର *errmsg = "Couldn't parse %s: %s\n";

	ଯଦି ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	ଯଦି (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ଯଦି (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} ଅନ୍ୟଥା {
		major = strtoul(argv[1], &tmp, 0);
		ଯଦି (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ଯଦି (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	ଯଦି (devname) {
		printf("Found device!  %s\n", devname);
		ମୁକ୍ତ(devname);
	} ଅନ୍ୟଥା {
		printf("Couldn't find device.\n");
	}
	ଫେରନ୍ତୁ 0;
}

#endif
