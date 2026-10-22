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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct link_struct  {
	const char	*name;
	số_nguyên		namelen;
	ext2_ino_t	inode;
	số_nguyên		flags;
	struct ext2_dir_entry *prev;
	số_nguyên		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static số_nguyên unlink_proc(struct ext2_dir_entry *dirent,
		     số_nguyên	offset,
		     số_nguyên	blocksize EXT2FS_ATTR((unused)),
		     char	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	nếu (ls->name) {
		nếu (ext2fs_dirent_name_len(dirent) != ls->namelen)
			trả_về 0;
		nếu (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			trả_về 0;
	}
	nếu (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		nếu (dirent->inode != ls->inode)
			trả_về 0;
	} không_thì {
		nếu (!dirent->inode)
			trả_về 0;
	}

	nếu (offset)
		prev->rec_len += dirent->rec_len;
	không_thì
		dirent->inode = 0;
	ls->done++;
	trả_về DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const char *name, ext2_ino_t ino,
			số_nguyên flags)
{
	errcode_t	retval;
	struct link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	nếu (!name && !ino)
		trả_về EXT2_ET_INVALID_ARGUMENT;

	nếu (!(fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	nếu (retval)
		trả_về retval;

	trả_về (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

