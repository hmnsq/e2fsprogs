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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <stdlib.h>
#समावेश <string.h>
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#समावेश <dirent.h>
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#अगर HAVE_SYS_MKDEV_H
#समावेश <sys/mkdev.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_SYSMACROS_H
#समावेश <sys/sysmacros.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

काष्ठा dir_list {
	अक्षर	*name;
	काष्ठा dir_list *अग्र;
};

/*
 * This function adds an entry to the directory list
 */
अटल व्योम add_to_dirlist(स्थिर अक्षर *name, काष्ठा dir_list **list)
{
	काष्ठा dir_list *dp;

	dp = दो_स्मृति(माप(काष्ठा dir_list));
	अगर (!dp)
		वापस;
	dp->name = दो_स्मृति(म_माप(name)+1);
	अगर (!dp->name) {
		मुक्त(dp);
		वापस;
	}
	म_नकल(dp->name, name);
	dp->अग्र = *list;
	*list = dp;
}

/*
 * This function frees a directory list
 */
अटल व्योम free_dirlist(काष्ठा dir_list **list)
{
	काष्ठा dir_list *dp, *अग्र;

	क्रम (dp = *list; dp; dp = अग्र) {
		अग्र = dp->अग्र;
		मुक्त(dp->name);
		मुक्त(dp);
	}
	*list = 0;
}

अटल पूर्णांक scan_dir(अक्षर *स_नाम, dev_t device, काष्ठा dir_list **list,
		    अक्षर **ret_path)
{
	सूची	*dir;
	काष्ठा dirent *dp;
	अक्षर	path[1024], *cp;
	पूर्णांक	dirlen;
	काष्ठा stat st;

	dirlen = म_माप(स_नाम);
	अगर ((dir = सूची_खोलो(स_नाम)) == शून्य)
		वापस त्रुटि_सं;
	dp = सूची_पढ़ो(dir);
	जबतक (dp) {
		अगर (dirlen + म_माप(dp->d_name) + 2 >= माप(path))
			जाओ skip_to_अग्र;
		अगर (dp->d_name[0] == '.' &&
		    ((dp->d_name[1] == 0) ||
		     ((dp->d_name[1] == '.') && (dp->d_name[2] == 0))))
			जाओ skip_to_अग्र;
		प्र_लिखो(path, "%s/%s", स_नाम, dp->d_name);
		अगर (stat(path, &st) < 0)
			जाओ skip_to_अग्र;
		अगर (S_ISDIR(st.st_mode))
			add_to_dirlist(path, list);
		अगर (ext2fsP_is_disk_device(st.st_mode) &&
		    st.st_rdev == device) {
			cp = दो_स्मृति(म_माप(path)+1);
			अगर (!cp) {
				बंद_सूची(dir);
				वापस ENOMEM;
			}
			म_नकल(cp, path);
			*ret_path = cp;
			जाओ success;
		}
	skip_to_अग्र:
		dp = सूची_पढ़ो(dir);
	}
success:
	बंद_सूची(dir);
	वापस 0;
}

/*
 * This function finds the pathname to a block device with a given
 * device number.  It returns a pointer to allocated memory to the
 * pathname on success, and NULL on failure.
 */
अक्षर *ext2fs_find_block_device(dev_t device)
{
	काष्ठा dir_list *list = 0, *new_list = 0;
	काष्ठा dir_list *current;
	अक्षर	*ret_path = 0;
	पूर्णांक    level = 0;

	/*
	 * Add the starting directories to search...
	 */
	add_to_dirlist("/devices", &list);
	add_to_dirlist("/devfs", &list);
	add_to_dirlist("/dev", &list);

	जबतक (list) {
		current = list;
		list = list->अग्र;
#अगर_घोषित DEBUG
		म_लिखो("Scanning directory %s\n", current->name);
#पूर्ण_अगर
		scan_dir(current->name, device, &new_list, &ret_path);
		मुक्त(current->name);
		मुक्त(current);
		अगर (ret_path)
			अवरोध;
		/*
		 * If we're done checking at this level, descend to
		 * the next level of subdirectories. (breadth-first)
		 */
		अगर (list == 0) {
			list = new_list;
			new_list = 0;
			/* Avoid infinite loop */
			अगर (++level >= EXT2FS_MAX_NESTED_LINKS)
				अवरोध;
		}
	}
	free_dirlist(&list);
	free_dirlist(&new_list);
	वापस ret_path;
}


#अगर_घोषित DEBUG
पूर्णांक मुख्य(पूर्णांक argc, अक्षर** argv)
{
	अक्षर	*devname, *tmp;
	पूर्णांक	major, minor;
	dev_t	device;
	स्थिर अक्षर *errmsg = "Couldn't parse %s: %s\n";

	अगर ((argc != 2) && (argc != 3)) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device_number\n", argv[0]);
		ख_लिखो(मानक_त्रुटि, "\t: %s major minor\n", argv[0]);
		निकास(1);
	}
	अगर (argc == 2) {
		device = म_से_अदीर्घ(argv[1], &tmp, 0);
		अगर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "device number", argv[1]);
			निकास(1);
		}
	} अन्यथा {
		major = म_से_अदीर्घ(argv[1], &tmp, 0);
		अगर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "major number", argv[1]);
			निकास(1);
		}
		minor = म_से_अदीर्घ(argv[2], &tmp, 0);
		अगर (*tmp) {
			ख_लिखो(मानक_त्रुटि, errmsg, "minor number", argv[2]);
			निकास(1);
		}
		device = makedev(major, minor);
		म_लिखो("Looking for device 0x%04x (%d:%d)\n", device,
		       major, minor);
	}
	devname = ext2fs_find_block_device(device);
	अगर (devname) {
		म_लिखो("Found device!  %s\n", devname);
		मुक्त(devname);
	} अन्यथा {
		म_लिखो("Couldn't find device.\n");
	}
	वापस 0;
}

#पूर्ण_अगर
