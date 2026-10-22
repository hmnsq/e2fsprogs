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
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساخت lookup_struct  {
	ثابت حرف	*name;
	صحیح		len;
	ext2_ino_t	*inode;
	صحیح		ملا;
};

#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح lookup_proc(ساخت ext2_dir_اندراج *dirent,
		       صحیح	انحراف EXT2FS_ATTR((غیر_مستعمل)),
		       صحیح	blocksize EXT2FS_ATTR((غیر_مستعمل)),
		       حرف	*buf EXT2FS_ATTR((غیر_مستعمل)),
		       خالی	*priv_ڈیٹا)
{
	ساخت lookup_struct *ls = (ساخت lookup_struct *) priv_ڈیٹا;

	اگر (ls->len != ext2fs_dirent_name_len(dirent))
		واپس 0;
	اگر (لڑی_موازنہ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		واپس 0;
	*ls->inode = dirent->inode;
	ls->ملا++;
	واپس DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
			صحیح namelen, حرف *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ساخت lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.ملا = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	اگر (retval)
		واپس retval;

	واپس (ls.ملا) ? 0 : EXT2_ET_FILE_NOT_ملا;
}


