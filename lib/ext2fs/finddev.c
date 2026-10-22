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
#यदि_अन्तः
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#अन्तर्भाव <dirent.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#यदि HAVE_SYS_MKDEV_H
#अन्तर्भाव <sys/mkdev.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_SYSMACROS_H
#अन्तर्भाव <sys/sysmacros.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना dir_list {
	वर्ण	*name;
	संरचना dir_list *अग्र;
};

/*
 * This function adds an entry to the directory list
 */
स्थिर शून्य सङ्कलय_गन्तव्यम्_dirlist(ध्रुव वर्ण *name, संरचना dir_list **list)
{
	संरचना dir_list *dp;

	dp = स्मृतिं_देहि(परिमाण(संरचना dir_list));
	यदि (!dp)
		प्रत्यागम;
	dp->name = स्मृतिं_देहि(वाक्यदैर्घ्यम्(name)+1);
	यदि (!dp->name) {
		मुक्त(dp);
		प्रत्यागम;
	}
	वाक्यं_प्रतिलिख(dp->name, name);
	dp->अग्र = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
स्थिर शून्य free_dirlist(संरचना dir_list **list)
{
	संरचना dir_list *dp, *अग्र;

	प्रति (dp = *list; dp; dp = अग्र) {
		अग्र = dp->अग्र;
		मुक्त(dp->name);
		मुक्त(dp);
	}
	*list = 0;
}

स्थिर पूर्णाङ्क अवलोकनम्_dir(वर्ण *dirname, dev_t device, संरचना dir_list **list,
		    वर्ण **ret_मार्गः)
{
	DIR	*dir;
	संरचना dirent *dp;
	वर्ण	मार्गः[1024], *cp;
	पूर्णाङ्क	dirlen;
	संरचना stat st;

	dirlen = वाक्यदैर्घ्यम्(dirname);
	यदि ((dir = opendir(dirname)) == रिक्त)
		प्रत्यागम errno;
	dp = readdir(dir);
	यावत् (dp) {
		यदि (dirlen + वाक्यदैर्घ्यम्(dp->d_name) + 2 >= परिमाण(मार्गः))
			गच्छ skip_गन्तव्यम्_अग्र;
		यदि (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			गच्छ skip_गन्तव्यम्_अग्र;
		sprintf(मार्गः, "%s/%s", dirname, dp->d_name);
		यदि (stat(मार्गः, &st) < 0)
			गच्छ skip_गन्तव्यम्_अग्र;
		यदि (S_ISDIR(st.st_बहुलकम्))
			सङ्कलय_गन्तव्यम्_dirlist(मार्गः, list);
		यदि (ext2fsP_is_disk_device(st.st_बहुलकम्) &&
		    st.st_rdev == device) {
			cp = स्मृतिं_देहि(वाक्यदैर्घ्यम्(मार्गः)+1);
			यदि (!cp) {
				closedir(dir);
				प्रत्यागम ENOMEM;
			}
			वाक्यं_प्रतिलिख(cp, मार्गः);
			*ret_मार्गः = cp;
			गच्छ success;
		}
	skip_गन्तव्यम्_अग्र:
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
वर्ण *ext2fs_find_खण्डम्_device(dev_t device)
{
	संरचना dir_list *list = 0, *new_list = 0;
	संरचना dir_list *current;
	वर्ण	*ret_मार्गः = 0;
	पूर्णाङ्क    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	सङ्कलय_गन्तव्यम्_dirlist("/devices", &list);
	सङ्कलय_गन्तव्यम्_dirlist("/devfs", &list);
	सङ्कलय_गन्तव्यम्_dirlist("/dev", &list);

	यावत् (list) {
		current = list;
		list = list->अग्र;
#यदि_परिभाषितम् DEBUG
		प्रारूप_लिख("Scanning directory %s\n", current->name);
#यदि_अन्तः
		अवलोकनम्_dir(current->name, device, &new_list, &ret_मार्गः);
		मुक्त(current->name);
		मुक्त(current);
		यदि (ret_मार्गः)
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
	प्रत्यागम ret_मार्गः;
}


#यदि_परिभाषितम् DEBUG
पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण** argv)
{
	वर्ण	*devname, *tmp;
	पूर्णाङ्क	major, minor;
	dev_t	device;
	ध्रुव वर्ण *errmsg = "Couldn't parse %s: %s\n";

	यदि ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "Usage: %s device_number\n", argv[0]);
		fprintf(stderr, "\t: %s major minor\n", argv[0]);
		निर्गच्छ(1);
	}
	यदि (argc == 2) {
		device = strtoul(argv[1], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "device number", argv[1]);
			निर्गच्छ(1);
		}
	} अन्यथा {
		major = strtoul(argv[1], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "major number", argv[1]);
			निर्गच्छ(1);
		}
		minor = strtoul(argv[2], &tmp, 0);
		यदि (*tmp) {
			fprintf(stderr, errmsg, "minor number", argv[2]);
			निर्गच्छ(1);
		}
		device = makedev(major, minor);
		प्रारूप_लिख("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_खण्डम्_device(device);
	यदि (devname) {
		प्रारूप_लिख("Found device!  %s\n", devname);
		मुक्त(devname);
	} अन्यथा {
		प्रारूप_लिख("Couldn't find device.\n");
	}
	प्रत्यागम 0;
}

#यदि_अन्तः
