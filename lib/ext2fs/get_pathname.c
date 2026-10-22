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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

কাঠামো get_pathname_struct {
	ext2_ino_t	অনুসন্ধান_ino;
	ext2_ino_t	parent;
	অক্ষর		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণসংখ্যা get_pathname_proc(কাঠামো ext2_dir_entry *dirent,
			     পূর্ণসংখ্যা	offset EXT2FS_ATTR((unused)),
			     পূর্ণসংখ্যা	blocksize EXT2FS_ATTR((unused)),
			     অক্ষর	*buf EXT2FS_ATTR((unused)),
			     শূন্য	*priv_তথ্য)
{
	কাঠামো get_pathname_struct	*gp;
	errcode_t			retval;
	পূর্ণসংখ্যা name_len = ext2fs_dirent_name_len(dirent);

	gp = (কাঠামো get_pathname_struct *) priv_তথ্য;

	যদি ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	যদি (dirent->inode == gp->অনুসন্ধান_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		যদি (retval) {
			gp->errcode = retval;
			ফেরত DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ফেরত DIRENT_ABORT;
	}
	ফেরত 0;
}

স্থির errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, পূর্ণসংখ্যা maxdepth,
					 অক্ষর *buf, অক্ষর **name)
{
	কাঠামো get_pathname_struct gp;
	অক্ষর	*parent_name = 0, *ret;
	errcode_t	retval;

	যদি (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		যদি (retval)
			ফেরত retval;
		strcpy(*name, (dir == EXT2_মূল_INO) ? "/" : ".");
		ফেরত 0;
	}

	যদি (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		যদি (retval)
			ফেরত retval;
		strcpy(*name, "...");
		ফেরত 0;
	}

	gp.অনুসন্ধান_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	যদি (retval == EXT2_ET_NO_DIRECTORY) {
		অক্ষর tmp[32];

		যদি (ino)
			snprintf(tmp, আকার(tmp), "<%u>/<%u>", dir, ino);
		অন্যথা
			snprintf(tmp, আকার(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		যদি (retval)
			যাও cleanup;
		strcpy(*name, tmp);
		ফেরত 0;
	} অন্যথা যদি (retval)
		যাও cleanup;
	যদি (gp.errcode) {
		retval = gp.errcode;
		যাও cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	যদি (retval)
		যাও cleanup;
	যদি (!ino) {
		*name = parent_name;
		ফেরত 0;
	}

	যদি (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	অন্যথা
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	যদি (retval)
		যাও cleanup;

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
	ফেরত retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      অক্ষর **name)
{
	অক্ষর	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;
	যদি (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ফেরত retval;

}
