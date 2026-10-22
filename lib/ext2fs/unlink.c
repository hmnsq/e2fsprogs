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
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية link_struct  {
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	بنية ext2_dir_entry *prev;
	صحيح		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ساكن صحيح unlink_proc(بنية ext2_dir_entry *dirent,
		     صحيح	offset,
		     صحيح	blocksize EXT2FS_ATTR((unused)),
		     حرف	*buf EXT2FS_ATTR((unused)),
		     فراغ	*priv_data)
{
	بنية link_struct *ls = (بنية link_struct *) priv_data;
	بنية ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	إذا (ls->name) {
		إذا (ext2fs_dirent_name_len(dirent) != ls->namelen)
			إرجاع 0;
		إذا (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			إرجاع 0;
	}
	إذا (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		إذا (dirent->inode != ls->inode)
			إرجاع 0;
	} وإلا {
		إذا (!dirent->inode)
			إرجاع 0;
	}

	إذا (offset)
		prev->rec_len += dirent->rec_len;
	وإلا
		dirent->inode = 0;
	ls->done++;
	إرجاع DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ثابت حرف *name, ext2_ino_t ino,
			صحيح flags)
{
	errcode_t	retval;
	بنية link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!name && !ino)
		إرجاع EXT2_ET_INVALID_ARGUMENT;

	إذا (!(fs->flags & EXT2_FLAG_RW))
		إرجاع EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	إذا (retval)
		إرجاع retval;

	إرجاع (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

