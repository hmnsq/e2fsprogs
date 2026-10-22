/*
 * get_pathname.c --- do directory/inode -> name translation
 *
 * Copyright (C) 1993, 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

/*
 *
 * 	ext2fs_get_pathname(fs, dir, ino, name)
 *
 * 	This function translates takes two inode numbers into a
 * 	string, placing the result in <name>.  <dir> is the containing
 * 	directory inode, and <ino> is the inode number itself.  If
 * 	<ino> is zero, then ext2fs_get_pathname will return pathname
 * 	of the the directory <dir>.
 *
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

সংরচনা get_pathname_struct {
	ext2_ino_t	অনুসন্ধান_ino;
	ext2_ino_t	parent;
	ৱর্ণ		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক get_pathname_proc(সংরচনা ext2_dir_entry *dirent,
			     পূর্ণাঙ্ক	offset EXT2FS_ATTR((unused)),
			     পূর্ণাঙ্ক	blocksize EXT2FS_ATTR((unused)),
			     ৱর্ণ	*buf EXT2FS_ATTR((unused)),
			     শূন্য	*priv_data)
{
	সংরচনা get_pathname_struct	*gp;
	errcode_t			retval;
	পূর্ণাঙ্ক name_len = ext2fs_dirent_name_len(dirent);

	gp = (সংরচনা get_pathname_struct *) priv_data;

	যদি ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	যদি (dirent->inode == gp->অনুসন্ধান_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		যদি (retval) {
			gp->errcode = retval;
			ঘূৰাই DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ঘূৰাই DIRENT_ABORT;
	}
	ঘূৰাই 0;
}

স্থির errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, পূর্ণাঙ্ক maxdepth,
					 ৱর্ণ *buf, ৱর্ণ **name)
{
	সংরচনা get_pathname_struct gp;
	ৱর্ণ	*parent_name = 0, *ret;
	errcode_t	retval;

	যদি (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		যদি (retval)
			ঘূৰাই retval;
		strcpy(*name, (dir == EXT2_মূল_INO) ? "/" : ".");
		ঘূৰাই 0;
	}

	যদি (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		যদি (retval)
			ঘূৰাই retval;
		strcpy(*name, "...");
		ঘূৰাই 0;
	}

	gp.অনুসন্ধান_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	যদি (retval == EXT2_ET_NO_DIRECTORY) {
		ৱর্ণ tmp[32];

		যদি (ino)
			snprintf(tmp, পরিমাণ(tmp), "<%u>/<%u>", dir, ino);
		অন্যথা
			snprintf(tmp, পরিমাণ(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		যদি (retval)
			গচ্ছ cleanup;
		strcpy(*name, tmp);
		ঘূৰাই 0;
	} অন্যথা যদি (retval)
		গচ্ছ cleanup;
	যদি (gp.errcode) {
		retval = gp.errcode;
		গচ্ছ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	যদি (retval)
		গচ্ছ cleanup;
	যদি (!ino) {
		*name = parent_name;
		ঘূৰাই 0;
	}

	যদি (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	অন্যথা
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	যদি (retval)
		গচ্ছ cleanup;

	ret[0] = 0;
	যদি (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	যদি (gp.name)
		strcat(ret, gp.name);
	অন্যথা
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	ঘূৰাই retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      ৱর্ণ **name)
{
	ৱর্ণ	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ঘূৰাই retval;
	যদি (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ঘূৰাই retval;

}
