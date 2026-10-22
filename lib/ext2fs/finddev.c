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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#અન્તર્ભાવ <dirent.h>
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#જો HAVE_SYS_MKDEV_H
#અન્તર્ભાવ <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#અન્તર્ભાવ <sys/sysmacros.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના dir_list {
	અક્ષર	*name;
	સંરચના dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
સ્થિર શૂન્ય add_to_dirlist(અચલ અક્ષર *name, સંરચના dir_list **list)
{
	સંરચના dir_list *dp;

	dp = malloc(કદ(સંરચના dir_list));
	જો (!dp)
		પાછા;
	dp->name = malloc(strlen(name)+1);
	જો (!dp->name) {
		મુક્ત(dp);
		પાછા;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
સ્થિર શૂન્ય free_dirlist(સંરચના dir_list **list)
{
	સંરચના dir_list *dp, *next;

	માટે (dp = *list; dp; dp = next) {
		next = dp->next;
		મુક્ત(dp->name);
		મુક્ત(dp);
	}
	*list = 0;
}

સ્થિર પૂર્ણાંક scan_dir(અક્ષર *dirname, dev_t device, સંરચના dir_list **list,
		    અક્ષર **ret_path)
{
	DIR	*dir;
	સંરચના dirent *dp;
	અક્ષર	path[1024], *cp;
	પૂર્ણાંક	dirlen;
	સંરચના stat st;

	dirlen = strlen(dirname);
	જો ((dir = opendir(dirname)) == ખાલી)
		પાછા errno;
	dp = readdir(dir);
	જ્યારે (dp) {
		જો (dirlen + strlen(dp->d_name) + 2 >= કદ(path))
			જાઓ skip_to_next;
		જો (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			જાઓ skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		જો (stat(path, &st) < 0)
			જાઓ skip_to_next;
		જો (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		જો (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			જો (!cp) {
				closedir(dir);
				પાછા ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			જાઓ success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	પાછા 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
અક્ષર *ext2fs_find_block_device(dev_t device)
{
	સંરચના dir_list *list = 0, *new_list = 0;
	સંરચના dir_list *current;
	અક્ષર	*ret_path = 0;
	પૂર્ણાંક    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	જ્યારે (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		મુક્ત(current->name);
		મુક્ત(current);
		જો (ret_path)
			તોડો;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		જો (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			જો (++level >= EXT2FS_MAX_NESTED_LINKS)
				તોડો;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	પાછા ret_path;
}


#ifdef DEBUG
પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર** argv)
{
	અક્ષર	*devname, *tmp;
	પૂર્ણાંક	major, minor;
	dev_t	device;
	અચલ અક્ષર *errmsg = "Couldn't parse %s: %s\n";

	જો ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	જો (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		જો (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} નહીંતર {
		major = strtoul(argv[1], &tmp, 0);
		જો (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		જો (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	જો (devname) {
		printf("Found device!  %s\n", devname);
		મુક્ત(devname);
	} નહીંતર {
		printf("Couldn't find device.\n");
	}
	પાછા 0;
}

#endif
