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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

সংরচনা link_struct  {
	ধ্রুব বর্ণ	*name;
	পূর্ণাঙ্ক		namelen;
	ext2_ino_t	inode;
	পূর্ণাঙ্ক		flags;
	সংরচনা ext2_dir_entry *prev;
	পূর্ণাঙ্ক		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক unlink_proc(সংরচনা ext2_dir_entry *dirent,
		     পূর্ণাঙ্ক	offset,
		     পূর্ণাঙ্ক	blocksize EXT2FS_ATTR((unused)),
		     বর্ণ	*buf EXT2FS_ATTR((unused)),
		     শূন্য	*priv_data)
{
	সংরচনা link_struct *ls = (সংরচনা link_struct *) priv_data;
	সংরচনা ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	যদি (ls->name) {
		যদি (ext2fs_dirent_name_len(dirent) != ls->namelen)
			প্রত্যাগম 0;
		যদি (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			প্রত্যাগম 0;
	}
	যদি (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		যদি (dirent->inode != ls->inode)
			প্রত্যাগম 0;
	} অন্যথা {
		যদি (!dirent->inode)
			প্রত্যাগম 0;
	}

	যদি (offset)
		prev->rec_len += dirent->rec_len;
	অন্যথা
		dirent->inode = 0;
	ls->done++;
	প্রত্যাগম DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ধ্রুব বর্ণ *name, ext2_ino_t ino,
			পূর্ণাঙ্ক flags)
{
	errcode_t	retval;
	সংরচনা link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!name && !ino)
		প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;

	যদি (!(fs->flags & EXT2_FLAG_RW))
		প্রত্যাগম EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	যদি (retval)
		প্রত্যাগম retval;

	প্রত্যাগম (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

