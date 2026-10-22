/*
 * unlink.c --- delete links in a ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1997 Theodore Ts'o.
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

ساخت link_struct  {
	ثابت حرف	*name;
	صحیح		namelen;
	ext2_ino_t	inode;
	صحیح		flags;
	ساخت ext2_dir_اندراج *پچھلا;
	صحیح		مکمل;
};

#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح unlink_proc(ساخت ext2_dir_اندراج *dirent,
		     صحیح	انحراف,
		     صحیح	blocksize EXT2FS_ATTR((غیر_مستعمل)),
		     حرف	*buf EXT2FS_ATTR((غیر_مستعمل)),
		     خالی	*priv_ڈیٹا)
{
	ساخت link_struct *ls = (ساخت link_struct *) priv_ڈیٹا;
	ساخت ext2_dir_اندراج *پچھلا;

	پچھلا = ls->پچھلا;
	ls->پچھلا = dirent;

	اگر (ls->name) {
		اگر (ext2fs_dirent_name_len(dirent) != ls->namelen)
			واپس 0;
		اگر (لڑی_موازنہ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			واپس 0;
	}
	اگر (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		اگر (dirent->inode != ls->inode)
			واپس 0;
	} ورنہ {
		اگر (!dirent->inode)
			واپس 0;
	}

	اگر (انحراف)
		پچھلا->rec_len += dirent->rec_len;
	ورنہ
		dirent->inode = 0;
	ls->مکمل++;
	واپس DIRENT_ABORT|DIRENT_بدلا;
}

#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ثابت حرف *name, ext2_ino_t ino,
			صحیح flags)
{
	errcode_t	retval;
	ساخت link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!name && !ino)
		واپس EXT2_ET_INVALID_دلیل;

	اگر (!(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? لڑی_لمبائی(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.مکمل = 0;
	ls.پچھلا = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_خالی_خانے,
				    0, unlink_proc, &ls);
	اگر (retval)
		واپس retval;

	واپس (ls.مکمل) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

