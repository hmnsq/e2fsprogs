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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_MKDEV_H
#ಅನ್ತರ್ಭಾವ <sys/mkdev.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_SYSMACROS_H
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ dir_list {
	ಅಕ್ಷರ	*name;
	ರಚನೆ dir_list *ಮುಂದಿನ;
};

/*
 * This function adds an entry to the directory list
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_ಗಮ್ಯ_dirlist(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ರಚನೆ dir_list **list)
{
	ರಚನೆ dir_list *dp;

	dp = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ dir_list));
	ಆದರೆ (!dp)
		ಹಿಂತಿರುಗಿ;
	dp->name = ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1);
	ಆದರೆ (!dp->name) {
		ಬಿಡುಗಡೆ(dp);
		ಹಿಂತಿರುಗಿ;
	}
	strcpy(dp->name, name);
	dp->ಮುಂದಿನ = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
ಸ್ಥಿರ ಶೂನ್ಯ free_dirlist(ರಚನೆ dir_list **list)
{
	ರಚನೆ dir_list *dp, *ಮುಂದಿನ;

	ಗಾಗಿ (dp = *list; dp; dp = ಮುಂದಿನ) {
		ಮುಂದಿನ = dp->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(dp->name);
		ಬಿಡುಗಡೆ(dp);
	}
	*list = 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ_dir(ಅಕ್ಷರ *dirname, dev_t device, ರಚನೆ dir_list **list,
		    ಅಕ್ಷರ **ret_ಮಾರ್ಗ)
{
	DIR	*dir;
	ರಚನೆ dirent *dp;
	ಅಕ್ಷರ	ಮಾರ್ಗ[1024], *cp;
	ಪೂರ್ಣಾಂಕ	dirlen;
	ರಚನೆ stat st;

	dirlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dirname);
	ಆದರೆ ((dir = opendir(dirname)) == ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ errno;
	dp = readdir(dir);
	ಆಗ (dp) {
		ಆದರೆ (dirlen + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dp->d_name) + 2 >= ಗಾತ್ರ(ಮಾರ್ಗ))
			ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ;
		ಆದರೆ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ;
		sprintf(ಮಾರ್ಗ, "%s/%s", dirname, dp->d_name);
		ಆದರೆ (stat(ಮಾರ್ಗ, &st) < 0)
			ಹೋಗು skip_ಗಮ್ಯ_ಮುಂದಿನ;
		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ))
			ಕೂಡಿಸು_ಗಮ್ಯ_dirlist(ಮಾರ್ಗ, list);
		ಆದರೆ (ext2fsP_is_disk_device(st.st_ಬಹುಲಕ) &&
		    st.st_rdev == device) {
			cp = ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಮಾರ್ಗ)+1);
			ಆದರೆ (!cp) {
				closedir(dir);
				ಹಿಂತಿರುಗಿ ENOMEM;
			}
			strcpy(cp, ಮಾರ್ಗ);
			*ret_ಮಾರ್ಗ = cp;
			ಹೋಗು success;
		}
	skip_ಗಮ್ಯ_ಮುಂದಿನ:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
ಅಕ್ಷರ *ext2fs_ಕಂಡುಹಿಡಿ_ಖಂಡ_device(dev_t device)
{
	ರಚನೆ dir_list *list = 0, *new_list = 0;
	ರಚನೆ dir_list *current;
	ಅಕ್ಷರ	*ret_ಮಾರ್ಗ = 0;
	ಪೂರ್ಣಾಂಕ    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	ಕೂಡಿಸು_ಗಮ್ಯ_dirlist("/devices", &list);
	ಕೂಡಿಸು_ಗಮ್ಯ_dirlist("/devfs", &list);
	ಕೂಡಿಸು_ಗಮ್ಯ_dirlist("/dev", &list);

	ಆಗ (list) {
		current = list;
		list = list->ಮುಂದಿನ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("Scanning directory %s\n", current->name);
#ಆದರೆ_ಅಂತ್ಯ
		ಪರಿಶೀಲನೆ_dir(current->name, device, &new_list, &ret_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(current->name);
		ಬಿಡುಗಡೆ(current);
		ಆದರೆ (ret_ಮಾರ್ಗ)
			ನಿಲ್ಲಿಸು;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ಆದರೆ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ಆದರೆ (++level >= EXT2FS_MAX_NESTED_LINKS)
				ನಿಲ್ಲಿಸು;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	ಹಿಂತಿರುಗಿ ret_ಮಾರ್ಗ;
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ** argv)
{
	ಅಕ್ಷರ	*devname, *tmp;
	ಪೂರ್ಣಾಂಕ	major, minor;
	dev_t	device;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *errmsg = "Couldn't parse %s: %s\n";

	ಆದರೆ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ಆದರೆ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			ನಿರ್ಗಮಿಸು(1);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		major = strtoul(argv[1], &tmp, 0);
		ಆದರೆ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			ನಿರ್ಗಮಿಸು(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ಆದರೆ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			ನಿರ್ಗಮಿಸು(1);
		}
		device = makedev(major, minor);
		ಬರೆ("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_ಕಂಡುಹಿಡಿ_ಖಂಡ_device(device);
	ಆದರೆ (devname) {
		ಬರೆ("Found device!  %s\n", devname);
		ಬಿಡುಗಡೆ(devname);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("Couldn't find device.\n");
	}
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ
