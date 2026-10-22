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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <stdlib.h>
#ضمن <string.h>
#𐤀𐤌 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#𐤀𐤌 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ضمن <dirent.h>
#𐤀𐤌 HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#𐤀𐤌 HAVE_SYS_MKDEV_H
#ضمن <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#ضمن <sys/sysmacros.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية dir_list {
	حرف	*name;
	بنية dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
ساكن فراغ add_to_dirlist(ثابت حرف *name, بنية dir_list **list)
{
	بنية dir_list *dp;

	dp = malloc(حجم(بنية dir_list));
	𐤀𐤌 (!dp)
		أرجع;
	dp->name = malloc(strlen(name)+1);
	𐤀𐤌 (!dp->name) {
		حرر(dp);
		أرجع;
	}
	strcpy(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ساكن فراغ free_dirlist(بنية dir_list **list)
{
	بنية dir_list *dp, *next;

	لكل (dp = *list; dp; dp = next) {
		next = dp->next;
		حرر(dp->name);
		حرر(dp);
	}
	*list = 0;
}

ساكن صحيح scan_dir(حرف *dirname, dev_t device, بنية dir_list **list,
		    حرف **ret_path)
{
	DIR	*dir;
	بنية dirent *dp;
	حرف	path[1024], *cp;
	صحيح	dirlen;
	بنية stat st;

	dirlen = strlen(dirname);
	𐤀𐤌 ((dir = opendir(dirname)) == عدم)
		أرجع errno;
	dp = readdir(dir);
	طالما (dp) {
		𐤀𐤌 (dirlen + strlen(dp->d_name) + 2 >= حجم(path))
			اذهب skip_to_next;
		𐤀𐤌 (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			اذهب skip_to_next;
		sprintf(path, "%s/%s", dirname, dp->d_name);
		𐤀𐤌 (stat(path, &st) < 0)
			اذهب skip_to_next;
		𐤀𐤌 (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		𐤀𐤌 (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = malloc(strlen(path)+1);
			𐤀𐤌 (!cp) {
				closedir(dir);
				أرجع ENOMEM;
			}
			strcpy(cp, path);
			*ret_path = cp;
			اذهب success;
		}
	skip_to_next:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	أرجع 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
حرف *ext2fs_find_block_device(dev_t device)
{
	بنية dir_list *list = 0, *new_list = 0;
	بنية dir_list *current;
	حرف	*ret_path = 0;
	صحيح    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	طالما (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		printf("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		حرر(current->name);
		حرر(current);
		𐤀𐤌 (ret_path)
			اكسر;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		𐤀𐤌 (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			𐤀𐤌 (++level >= EXT2FS_MAX_NESTED_LINKS)
				اكسر;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	أرجع ret_path;
}


#ifdef DEBUG
صحيح main(صحيح argc, حرف** argv)
{
	حرف	*devname, *tmp;
	صحيح	major, minor;
	dev_t	device;
	ثابت حرف *errmsg = "Couldn't parse %s: %s\n";

	𐤀𐤌 ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		exit(1);
	}
	𐤀𐤌 (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		𐤀𐤌 (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			exit(1);
		}
	} وإلا {
		major = strtoul(argv[1], &tmp, 0);
		𐤀𐤌 (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			exit(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		𐤀𐤌 (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			exit(1);
		}
		device = makedev(major, minor);
		printf("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	𐤀𐤌 (devname) {
		printf("Found device!  %s\n", devname);
		حرر(devname);
	} وإلا {
		printf("Couldn't find device.\n");
	}
	أرجع 0;
}

#endif
