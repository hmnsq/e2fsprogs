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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa link_struct  {
	const var_na	*name;
	ཧྲིལ		namelen;
	ext2_ino_t	inode;
	ཧྲིལ		flags;
	sa_nracanaa ext2_dir_entry *prev;
	ཧྲིལ		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ཧྲིལ unlink_proc(sa_nracanaa ext2_dir_entry *dirent,
		     ཧྲིལ	offset,
		     ཧྲིལ	blocksize EXT2FS_ATTR((unused)),
		     var_na	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	གལ་ཏེ (ls->name) {
		གལ་ཏེ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ཕྱིར 0;
		གལ་ཏེ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ཕྱིར 0;
	}
	གལ་ཏེ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		གལ་ཏེ (dirent->inode != ls->inode)
			ཕྱིར 0;
	} དེ་མིན {
		གལ་ཏེ (!dirent->inode)
			ཕྱིར 0;
	}

	གལ་ཏེ (offset)
		prev->rec_len += dirent->rec_len;
	དེ་མིན
		dirent->inode = 0;
	ls->done++;
	ཕྱིར DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const var_na *name, ext2_ino_t ino,
			ཧྲིལ flags)
{
	errcode_t	retval;
	sa_nracanaa link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	གལ་ཏེ (!name && !ino)
		ཕྱིར EXT2_ET_INVALID_ARGUMENT;

	གལ་ཏེ (!(fs->flags & EXT2_FLAG_RW))
		ཕྱིར EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	ཕྱིར (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

