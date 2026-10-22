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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية link_struct  {
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	بنية ext2_dir_مدخل *prev;
	صحيح		انجامشده;
};

#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
ساكن صحيح unlink_proc(بنية ext2_dir_مدخل *dirent,
		     صحيح	جابه_جایی,
		     صحيح	blocksize EXT2FS_ATTR((استفادهنشده)),
		     حرف	*buf EXT2FS_ATTR((استفادهنشده)),
		     فراغ	*priv_داده)
{
	بنية link_struct *ls = (بنية link_struct *) priv_داده;
	بنية ext2_dir_مدخل *prev;

	prev = ls->prev;
	ls->prev = dirent;

	إذا (ls->name) {
		إذا (ext2fs_dirent_name_len(dirent) != ls->namelen)
			أرجع 0;
		إذا (مقایسه_رشته(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			أرجع 0;
	}
	إذا (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		إذا (dirent->inode != ls->inode)
			أرجع 0;
	} وإلا {
		إذا (!dirent->inode)
			أرجع 0;
	}

	إذا (جابه_جایی)
		prev->rec_len += dirent->rec_len;
	وإلا
		dirent->inode = 0;
	ls->انجامشده++;
	أرجع DIRENT_ABORT|DIRENT_تغییریافته;
}

#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ثابت حرف *name, ext2_ino_t ino,
			صحيح flags)
{
	errcode_t	retval;
	بنية link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!name && !ino)
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? طول_رشته(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.انجامشده = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_خالی,
				    0, unlink_proc, &ls);
	إذا (retval)
		أرجع retval;

	أرجع (ls.انجامشده) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

