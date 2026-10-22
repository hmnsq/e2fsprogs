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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية lookup_struct  {
	ثابت حرف	*name;
	صحيح		len;
	ext2_ino_t	*inode;
	صحيح		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ساكن صحيح lookup_proc(بنية ext2_dir_entry *dirent,
		       صحيح	offset EXT2FS_ATTR((unused)),
		       صحيح	blocksize EXT2FS_ATTR((unused)),
		       حرف	*buf EXT2FS_ATTR((unused)),
		       فراغ	*priv_data)
{
	بنية lookup_struct *ls = (بنية lookup_struct *) priv_data;

	إذا (ls->len != ext2fs_dirent_name_len(dirent))
		أرجع 0;
	إذا (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		أرجع 0;
	*ls->inode = dirent->inode;
	ls->found++;
	أرجع DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			صحيح namelen, حرف *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	بنية lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	إذا (retval)
		أرجع retval;

	أرجع (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


