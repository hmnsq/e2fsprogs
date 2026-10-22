/*
 * lookup.c --- ext2fs directory lookup operations
 *
 * Copyright (C) 1993, 1994, 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساختار lookup_struct  {
	ثابت نویسه	*name;
	صحیح		len;
	ext2_ino_t	*inode;
	صحیح		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ایستا صحیح lookup_proc(ساختار ext2_dir_entry *dirent,
		       صحیح	offset EXT2FS_ATTR((unused)),
		       صحیح	blocksize EXT2FS_ATTR((unused)),
		       نویسه	*buf EXT2FS_ATTR((unused)),
		       تهی	*priv_data)
{
	ساختار lookup_struct *ls = (ساختار lookup_struct *) priv_data;

	اگر (ls->len != ext2fs_dirent_name_len(dirent))
		بازگشت 0;
	اگر (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		بازگشت 0;
	*ls->inode = dirent->inode;
	ls->found++;
	بازگشت DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت نویسه *name,
			صحیح namelen, نویسه *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ساختار lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	اگر (retval)
		بازگشت retval;

	بازگشت (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


