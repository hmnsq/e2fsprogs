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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_MKDEV_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mkdev.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_SYSMACROS_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ dir_list {
	ਅੱਖਰ	*name;
	ਬਣਤਰ dir_list *ਅਗਲਾ;
};

/*
 * This function adds an entry to the directory list
 */
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_dirlist(ਅਚਲ ਅੱਖਰ *name, ਬਣਤਰ dir_list **list)
{
	ਬਣਤਰ dir_list *dp;

	dp = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ dir_list));
	ਜੇ (!dp)
		ਵਾਪਸ;
	dp->name = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(name)+1);
	ਜੇ (!dp->name) {
		ਮੁਕਤ(dp);
		ਵਾਪਸ;
	}
	strcpy(dp->name, name);
	dp->ਅਗਲਾ = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ਸਥਿਰ ਖਾਲੀ free_dirlist(ਬਣਤਰ dir_list **list)
{
	ਬਣਤਰ dir_list *dp, *ਅਗਲਾ;

	ਲਈ (dp = *list; dp; dp = ਅਗਲਾ) {
		ਅਗਲਾ = dp->ਅਗਲਾ;
		ਮੁਕਤ(dp->name);
		ਮੁਕਤ(dp);
	}
	*list = 0;
}

ਸਥਿਰ ਪੂਰਨ ਜਾਂਚ_dir(ਅੱਖਰ *dirname, dev_t device, ਬਣਤਰ dir_list **list,
		    ਅੱਖਰ **ret_ਰਸਤਾ)
{
	DIR	*dir;
	ਬਣਤਰ dirent *dp;
	ਅੱਖਰ	ਰਸਤਾ[1024], *cp;
	ਪੂਰਨ	dirlen;
	ਬਣਤਰ stat st;

	dirlen = ਲੜੀ_ਲੰਬਾਈ(dirname);
	ਜੇ ((dir = opendir(dirname)) == ਨਿਰਾ)
		ਵਾਪਸ errno;
	dp = readdir(dir);
	ਜਦੋਂ (dp) {
		ਜੇ (dirlen + ਲੜੀ_ਲੰਬਾਈ(dp->d_name) + 2 >= ਆਕਾਰ(ਰਸਤਾ))
			ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ;
		ਜੇ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ;
		sprintf(ਰਸਤਾ, "%s/%s", dirname, dp->d_name);
		ਜੇ (stat(ਰਸਤਾ, &st) < 0)
			ਜਾਓ skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ;
		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ))
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_dirlist(ਰਸਤਾ, list);
		ਜੇ (ext2fsP_is_disk_device(st.st_ਬਹੁਲਕ) &&
		    st.st_rdev == device) {
			cp = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(ਰਸਤਾ)+1);
			ਜੇ (!cp) {
				closedir(dir);
				ਵਾਪਸ ENOMEM;
			}
			strcpy(cp, ਰਸਤਾ);
			*ret_ਰਸਤਾ = cp;
			ਜਾਓ success;
		}
	skip_ਮੰਜ਼ਿਲ_ਅਗਲਾ:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ਵਾਪਸ 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
ਅੱਖਰ *ext2fs_ਲੱਭੋ_ਖੰਡ_device(dev_t device)
{
	ਬਣਤਰ dir_list *list = 0, *new_list = 0;
	ਬਣਤਰ dir_list *current;
	ਅੱਖਰ	*ret_ਰਸਤਾ = 0;
	ਪੂਰਨ    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	ਜੋੜੋ_ਮੰਜ਼ਿਲ_dirlist("/devices", &list);
	ਜੋੜੋ_ਮੰਜ਼ਿਲ_dirlist("/devfs", &list);
	ਜੋੜੋ_ਮੰਜ਼ਿਲ_dirlist("/dev", &list);

	ਜਦੋਂ (list) {
		current = list;
		list = list->ਅਗਲਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("Scanning directory %s\n", current->name);
#ਜੇ_ਅੰਤ
		ਜਾਂਚ_dir(current->name, device, &new_list, &ret_ਰਸਤਾ);
		ਮੁਕਤ(current->name);
		ਮੁਕਤ(current);
		ਜੇ (ret_ਰਸਤਾ)
			ਰੋਕੋ;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ਜੇ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ਜੇ (++level >= EXT2FS_MAX_NESTED_LINKS)
				ਰੋਕੋ;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ਵਾਪਸ ret_ਰਸਤਾ;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ** argv)
{
	ਅੱਖਰ	*devname, *tmp;
	ਪੂਰਨ	major, minor;
	dev_t	device;
	ਅਚਲ ਅੱਖਰ *errmsg = "Couldn't parse %s: %s\n";

	ਜੇ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ਜੇ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	} ਨਹੀਂਤਾਂ {
		major = strtoul(argv[1], &tmp, 0);
		ਜੇ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ਜੇ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		device = makedev(major, minor);
		ਲਿਖੋ("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_ਲੱਭੋ_ਖੰਡ_device(device);
	ਜੇ (devname) {
		ਲਿਖੋ("Found device!  %s\n", devname);
		ਮੁਕਤ(devname);
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("Couldn't find device.\n");
	}
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ
