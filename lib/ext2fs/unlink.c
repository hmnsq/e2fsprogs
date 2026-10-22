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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct link_struct  {
	const char	*name;
	ціле		namelen;
	ext2_ino_t	inode;
	ціле		flags;
	struct ext2_dir_entry *prev;
	ціле		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ціле unlink_proc(struct ext2_dir_entry *dirent,
		     ціле	offset,
		     ціле	blocksize EXT2FS_ATTR((unused)),
		     char	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	якщо (ls->name) {
		якщо (ext2fs_dirent_name_len(dirent) != ls->namelen)
			повернути 0;
		якщо (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			повернути 0;
	}
	якщо (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		якщо (dirent->inode != ls->inode)
			повернути 0;
	} інакше {
		якщо (!dirent->inode)
			повернути 0;
	}

	якщо (offset)
		prev->rec_len += dirent->rec_len;
	інакше
		dirent->inode = 0;
	ls->done++;
	повернути DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const char *name, ext2_ino_t ino,
			ціле flags)
{
	errcode_t	retval;
	struct link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (!name && !ino)
		повернути EXT2_ET_INVALID_ARGUMENT;

	якщо (!(fs->flags & EXT2_FLAG_RW))
		повернути EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	якщо (retval)
		повернути retval;

	повернути (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

