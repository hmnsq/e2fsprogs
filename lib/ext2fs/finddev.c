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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#అన్తర్భావ <dirent.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_MKDEV_H
#అన్తర్భావ <sys/mkdev.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_SYSMACROS_H
#అన్తర్భావ <sys/sysmacros.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం dir_list {
	అక్షరం	*name;
	నిర్మాణం dir_list *తదుపరి;
};

/*
 * This function adds an entry to the directory list
 */
స్థిర శూన్య కూడు_గమ్యం_dirlist(స్థిరరాశి అక్షరం *name, నిర్మాణం dir_list **list)
{
	నిర్మాణం dir_list *dp;

	dp = జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం dir_list));
	ఒకవేళ (!dp)
		తిరిగి;
	dp->name = జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(name)+1);
	ఒకవేళ (!dp->name) {
		విడుదల(dp);
		తిరిగి;
	}
	strcpy(dp->name, name);
	dp->తదుపరి = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
స్థిర శూన్య free_dirlist(నిర్మాణం dir_list **list)
{
	నిర్మాణం dir_list *dp, *తదుపరి;

	కొరకు (dp = *list; dp; dp = తదుపరి) {
		తదుపరి = dp->తదుపరి;
		విడుదల(dp->name);
		విడుదల(dp);
	}
	*list = 0;
}

స్థిర పూర్ణాంకం పరిశీలన_dir(అక్షరం *dirname, dev_t device, నిర్మాణం dir_list **list,
		    అక్షరం **ret_మార్గం)
{
	DIR	*dir;
	నిర్మాణం dirent *dp;
	అక్షరం	మార్గం[1024], *cp;
	పూర్ణాంకం	dirlen;
	నిర్మాణం stat st;

	dirlen = అక్షరశ్రేణి_పొడవు(dirname);
	ఒకవేళ ((dir = opendir(dirname)) == ఖాళీ)
		తిరిగి errno;
	dp = readdir(dir);
	అయితే (dp) {
		ఒకవేళ (dirlen + అక్షరశ్రేణి_పొడవు(dp->d_name) + 2 >= పరిమాణం(మార్గం))
			వెళ్ళు skip_గమ్యం_తదుపరి;
		ఒకవేళ (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			వెళ్ళు skip_గమ్యం_తదుపరి;
		sprintf(మార్గం, "%s/%s", dirname, dp->d_name);
		ఒకవేళ (stat(మార్గం, &st) < 0)
			వెళ్ళు skip_గమ్యం_తదుపరి;
		ఒకవేళ (S_ISDIR(st.st_బహుళకం))
			కూడు_గమ్యం_dirlist(మార్గం, list);
		ఒకవేళ (ext2fsP_is_disk_device(st.st_బహుళకం) &&
		    st.st_rdev == device) {
			cp = జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(మార్గం)+1);
			ఒకవేళ (!cp) {
				closedir(dir);
				తిరిగి ENOMEM;
			}
			strcpy(cp, మార్గం);
			*ret_మార్గం = cp;
			వెళ్ళు success;
		}
	skip_గమ్యం_తదుపరి:
		dp = readdir(dir);
	}
success:
	closedir(dir);
	తిరిగి 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
అక్షరం *ext2fs_కనుగొను_ఖండం_device(dev_t device)
{
	నిర్మాణం dir_list *list = 0, *new_list = 0;
	నిర్మాణం dir_list *current;
	అక్షరం	*ret_మార్గం = 0;
	పూర్ణాంకం    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	కూడు_గమ్యం_dirlist("/devices", &list);
	కూడు_గమ్యం_dirlist("/devfs", &list);
	కూడు_గమ్యం_dirlist("/dev", &list);

	అయితే (list) {
		current = list;
		list = list->తదుపరి;
#నిర్వచితమైతే DEBUG
		రాయి("Scanning directory %s\n", current->name);
#అయితే_ముగింపు
		పరిశీలన_dir(current->name, device, &new_list, &ret_మార్గం);
		విడుదల(current->name);
		విడుదల(current);
		ఒకవేళ (ret_మార్గం)
			ఆపు;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		ఒకవేళ (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			ఒకవేళ (++level >= EXT2FS_MAX_NESTED_LINKS)
				ఆపు;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	తిరిగి ret_మార్గం;
}


#నిర్వచితమైతే DEBUG
పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం** argv)
{
	అక్షరం	*devname, *tmp;
	పూర్ణాంకం	major, minor;
	dev_t	device;
	స్థిరరాశి అక్షరం *errmsg = "Couldn't parse %s: %s\n";

	ఒకవేళ ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		నిష్క్రమించు(1);
	}
	ఒకవేళ (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		ఒకవేళ (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			నిష్క్రమించు(1);
		}
	} లేకపోతే {
		major = strtoul(argv[1], &tmp, 0);
		ఒకవేళ (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			నిష్క్రమించు(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		ఒకవేళ (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			నిష్క్రమించు(1);
		}
		device = makedev(major, minor);
		రాయి("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_కనుగొను_ఖండం_device(device);
	ఒకవేళ (devname) {
		రాయి("Found device!  %s\n", devname);
		విడుదల(devname);
	} లేకపోతే {
		రాయి("Couldn't find device.\n");
	}
	తిరిగి 0;
}

#అయితే_ముగింపు
