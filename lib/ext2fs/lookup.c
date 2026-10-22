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
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية lookup_struct  {
	ثابت حرف	*name;
	صحيح		len;
	ext2_ino_t	*inode;
	صحيح		لبھیا;
};

#جے_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#جے_انت
ساكن صحيح lookup_proc(بنية ext2_dir_اندراج *dirent,
		       صحيح	وتھ EXT2FS_ATTR((بن_ورتیا)),
		       صحيح	blocksize EXT2FS_ATTR((بن_ورتیا)),
		       حرف	*buf EXT2FS_ATTR((بن_ورتیا)),
		       فراغ	*priv_انکڑے)
{
	بنية lookup_struct *ls = (بنية lookup_struct *) priv_انکڑے;

	إذا (ls->len != ext2fs_dirent_name_len(dirent))
		أرجع 0;
	إذا (لڑی_مقابلہ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		أرجع 0;
	*ls->inode = dirent->inode;
	ls->لبھیا++;
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
	ls.لبھیا = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	إذا (retval)
		أرجع retval;

	أرجع (ls.لبھیا) ? 0 : EXT2_ET_FILE_NOT_لبھیا;
}


