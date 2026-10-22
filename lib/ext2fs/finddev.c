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
स्थिर शून्य add_to_dirlist(अचर वर्ण *name, संरचना dir_list **list)
{
	संरचना dir_list *dp;

	dp = दो_स्मृति(परिमाण(संरचना dir_list));
	यदि (!dp)
		फर्काऊ;
	dp->name = दो_स्मृति(म_माप(name)+1);
	यदि (!dp->name) {
		मुक्त(dp);
		फर्काऊ;
	}
	म_नकल(dp->name, name);
	dp->next = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
स्थिर शून्य free_dirlist(संरचना dir_list **list)
{
	संरचना dir_list *dp, *next;

	लागि (dp = *list; dp; dp = next) {
		next = dp->next;
		मुक्त(dp->name);
		मुक्त(dp);
	}
	*list = 0;
}

स्थिर पूर्णाङ्क scan_dir(वर्ण *स_नाम, dev_t device, संरचना dir_list **list,
		    वर्ण **ret_path)
{
	सूची	*dir;
	संरचना dirent *dp;
	वर्ण	path[1024], *cp;
	पूर्णाङ्क	dirlen;
	संरचना stat st;

	dirlen = म_माप(स_नाम);
	यदि ((dir = सूची_खोलो(स_नाम)) == रिक्त)
		फर्काऊ त्रुटि_सं;
	dp = सूची_पढ़ो(dir);
	जबसम्म (dp) {
		यदि (dirlen + म_माप(dp->d_name) + 2 >= परिमाण(path))
			गच्छ skip_to_next;
		यदि (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			गच्छ skip_to_next;
		प्र_लिखो(path, "%s/%s", स_नाम, dp->d_name);
		यदि (stat(path, &st) < 0)
			गच्छ skip_to_next;
		यदि (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		यदि (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = दो_स्मृति(म_माप(path)+1);
			यदि (!cp) {
				बंद_सूची(dir);
				फर्काऊ ENOMEM;
			}
			म_नकल(cp, path);
			*ret_path = cp;
			गच्छ success;
		}
	skip_to_next:
		dp = सूची_पढ़ो(dir);
	}
success:
	बंद_सूची(dir);
	फर्काऊ 0;
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

	जबसम्म (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		म_लिखो("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		मुक्त(current->name);
		मुक्त(current);
		यदि (ret_path)
			रोक;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		यदि (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			यदि (++level >= EXT2FS_MAX_NESTED_LINKS)
				रोक;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	फर्काऊ ret_path;
}


#ifdef DEBUG
पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण** argv)
{
	वर्ण	*devname, *tmp;
	पूर्णाङ्क	major, minor;
	dev_t	device;
	अचर वर्ण *errmsg = "Couldn't parse %s: %s\n";

	यदि ((argc != 2) && (argc != 3)) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device_number\n", argv[0]);
		ख_लिखो(मानक_त्रुटि, "\t: %s major minor\n", argv[0]);
		निकास(1);
	}
	यदि (argc == 2) {
		device = म_से_अदीर्घ(argv[1], &tmp, 0);
		यदि (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "device number", argv[1]);
			निकास(1);
		}
	} अन्यथा {
		major = म_से_अदीर्घ(argv[1], &tmp, 0);
		यदि (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "major number", argv[1]);
			निकास(1);
		}
		minor = म_से_अदीर्घ(argv[2], &tmp, 0);
		यदि (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "minor number", argv[2]);
			निकास(1);
		}
		device = makedev(major, minor);
		म_लिखो("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	यदि (devname) {
		म_लिखो("Found device!  %s\n", devname);
		मुक्त(devname);
	} अन्यथा {
		म_लिखो("Couldn't find device.\n");
	}
	फर्काऊ 0;
}

#endif
