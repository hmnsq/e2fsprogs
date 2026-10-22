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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <dirent.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#यदि HAVE_SYS_MKDEV_H
#अन्तर्भाव <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#अन्तर्भाव <sys/sysmacros.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना dir_list {
	वर्ण	*name;
	संरचना dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
स्थिर शून्य add_to_dirlist(ध्रुव वर्ण *name, संरचना dir_list **list)
{
	संरचना dir_list *dp;

	dp = malloc(परिमाण(संरचना dir_list));
	यदि (!dp)
		प्रत्यागम;
	dp->name = malloc(strlen(name)+1);
	यदि (!dp->name) {
		मुक्त(dp);
		प्रत्यागम;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
स्थिर शून्य free_dirlist(संरचना dir_list **list)
{
	संरचना dir_list *dp, *next;

	प्रति (dp = *list; dp; dp = next) {
		next = dp->next;
		मुक्त(dp->name);
		मुक्त(dp);
	}
	*list = 0;
}

स्थिर पूर्णाङ्क scan_dir(वर्ण *dirname, dev_t device, संरचना dir_list **list,
		    वर्ण **ret_path)
{
	DIR	*dir;
	संरचना dirent *dp;
	वर्ण	path[1024], *cp;
	पूर्णाङ्क	dirlen;
	संरचना stat st;

	dirlen = strlen(dirname);
	यदि ((dir = opendir(dirname)) == रिक्त)
		प्रत्यागम errno;
	dp = readdir(dir);
	यावत् (dp) {
		यदि (dirlen + strlen(dp->d_name) + 2 >= परिमाण(path))
			गच्छ skip_to_next;
		यदि (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			गच्छ skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		यदि (stat(path, &st) < 0)
			गच्छ skip_to_next;
		यदि (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		यदि (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			यदि (!cp) {
				closedir(dir);
				प्रत्यागम ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			गच्छ success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	प्रत्यागम 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
वर्ण *ext2fs_find_block_device(dev_t device)
{
	संरचना dir_list *list = 0, *new_list = 0;
	संरचना dir_list *current;
	वर्ण	*ret_path = 0;
	पूर्णाङ्क    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	यावत् (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		मुक्त(current->name);
		मुक्त(current);
		यदि (ret_path)
			भङ्ग;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		यदि (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			यदि (++level >= EXT2FS_MAX_NESTED_LINKS)
				भङ्ग;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	प्रत्यागम ret_path;
}


#ifdef DEBUG
पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण** argv)
{
	वर्ण	*devname, *tmp;
	पूर्णाङ्क	major, minor;
	dev_t	device;
	ध्रुव वर्ण *errmsg = "Couldn't parse %s: %s\n";

	यदि ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	यदि (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} अन्यथा {
		major = strtoul(argv[1], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	यदि (devname) {
		printf("Found device!  %s\n", devname);
		मुक्त(devname);
	} अन्यथा {
		printf("Couldn't find device.\n");
	}
	प्रत्यागम 0;
}

#endif
