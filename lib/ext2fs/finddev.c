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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#अन्तर्भाव <dirent.h>
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#जर HAVE_SYS_MKDEV_H
#अन्तर्भाव <sys/mkdev.h>
#endif
#ifdef HAVE_SYS_SYSMACROS_H
#अन्तर्भाव <sys/sysmacros.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना dir_list {
	अक्षर	*name;
	संरचना dir_list *next;
};

/*
 * This function adds an entry to the directory list
 */
स्थिर शून्य add_to_dirlist(स्थिरांक अक्षर *name, संरचना dir_list **list)
{
	संरचना dir_list *dp;

	dp = दो_स्मृति(आकार(संरचना dir_list));
	जर (!dp)
		परत;
	dp->name = दो_स्मृति(म_माप(name)+1);
	जर (!dp->name) {
		मुक्त(dp);
		परत;
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

	साठी (dp = *list; dp; dp = next) {
		next = dp->next;
		मुक्त(dp->name);
		मुक्त(dp);
	}
	*list = 0;
}

स्थिर पूर्णांक scan_dir(अक्षर *स_नाम, dev_t device, संरचना dir_list **list,
		    अक्षर **ret_path)
{
	सूची	*dir;
	संरचना dirent *dp;
	अक्षर	path[1024], *cp;
	पूर्णांक	dirlen;
	संरचना stat st;

	dirlen = म_माप(स_नाम);
	जर ((dir = सूची_खोलो(स_नाम)) == रिक्त)
		परत त्रुटि_सं;
	dp = सूची_पढ़ो(dir);
	पर्यंत (dp) {
		जर (dirlen + म_माप(dp->d_name) + 2 >= आकार(path))
			जा skip_to_next;
		जर (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			जा skip_to_next;
		प्र_लिखो(path, "%s/%s", स_नाम, dp->d_name);
		जर (stat(path, &st) < 0)
			जा skip_to_next;
		जर (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		जर (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = दो_स्मृति(म_माप(path)+1);
			जर (!cp) {
				बंद_सूची(dir);
				परत ENOMEM;
			}
			म_नकल(cp, path);
			*ret_path = cp;
			जा success;
		}
	skip_to_next:
		dp = सूची_पढ़ो(dir);
	}
success:
	बंद_सूची(dir);
	परत 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
अक्षर *ext2fs_find_block_device(dev_t device)
{
	संरचना dir_list *list = 0, *new_list = 0;
	संरचना dir_list *current;
	अक्षर	*ret_path = 0;
	पूर्णांक    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	पर्यंत (list) {
		current = list;
		list = list->next;
#ifdef DEBUG
		म_लिखो("Scanning directory %s\n", current->name);
#endif
		scan_dir(current->name, device, &new_list, &ret_path);
		मुक्त(current->name);
		मुक्त(current);
		जर (ret_path)
			थांबा;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		जर (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			जर (++level >= EXT2FS_MAX_NESTED_LINKS)
				थांबा;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	परत ret_path;
}


#ifdef DEBUG
पूर्णांक main(पूर्णांक argc, अक्षर** argv)
{
	अक्षर	*devname, *tmp;
	पूर्णांक	major, minor;
	dev_t	device;
	स्थिरांक अक्षर *errmsg = "Couldn't parse %s: %s\n";

	जर ((argc != 2) && (argc != 3)) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device_number\n", argv[0]);
		ख_लिखो(मानक_त्रुटि, "\t: %s major minor\n", argv[0]);
		निकास(1);
	}
	जर (argc == 2) {
		device = म_से_अदीर्घ(argv[1], &tmp, 0);
		जर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "device number", argv[1]);
			निकास(1);
		}
	} अन्यथा {
		major = म_से_अदीर्घ(argv[1], &tmp, 0);
		जर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "major number", argv[1]);
			निकास(1);
		}
		minor = म_से_अदीर्घ(argv[2], &tmp, 0);
		जर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "minor number", argv[2]);
			निकास(1);
		}
		device = makedev(major, minor);
		म_लिखो("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	जर (devname) {
		म_लिखो("Found device!  %s\n", devname);
		मुक्त(devname);
	} अन्यथा {
		म_लिखो("Couldn't find device.\n");
	}
	परत 0;
}

#endif
