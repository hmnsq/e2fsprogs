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
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية link_struct  {
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	بنية ext2_dir_ثبت *prev;
	صحيح		بشپړ;
};

#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
ساكن صحيح unlink_proc(بنية ext2_dir_ثبت *dirent,
		     صحيح	دننه_واټن,
		     صحيح	blocksize EXT2FS_ATTR((ناکارول_شوی)),
		     حرف	*buf EXT2FS_ATTR((ناکارول_شوی)),
		     فراغ	*priv_معلومات)
{
	بنية link_struct *ls = (بنية link_struct *) priv_معلومات;
	بنية ext2_dir_ثبت *prev;

	prev = ls->prev;
	ls->prev = dirent;

	إذا (ls->name) {
		إذا (ext2fs_dirent_name_len(dirent) != ls->namelen)
			أرجع 0;
		إذا (مزي_پرتله(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			أرجع 0;
	}
	إذا (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		إذا (dirent->inode != ls->inode)
			أرجع 0;
	} وإلا {
		إذا (!dirent->inode)
			أرجع 0;
	}

	إذا (دننه_واټن)
		prev->rec_len += dirent->rec_len;
	وإلا
		dirent->inode = 0;
	ls->بشپړ++;
	أرجع DIRENT_ABORT|DIRENT_بدل_شوی;
}

#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ثابت حرف *name, ext2_ino_t ino,
			صحيح flags)
{
	errcode_t	retval;
	بنية link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!name && !ino)
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? مزي_اوږدوالی(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.بشپړ = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_تشه,
				    0, unlink_proc, &ls);
	إذا (retval)
		أرجع retval;

	أرجع (ls.بشپړ) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

