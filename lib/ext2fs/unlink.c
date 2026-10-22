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
#если HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct link_struct  {
	const char	*name;
	целое		namelen;
	ext2_ino_t	inode;
	целое		flags;
	struct ext2_dir_entry *prev;
	целое		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static целое unlink_proc(struct ext2_dir_entry *dirent,
		     целое	offset,
		     целое	blocksize EXT2FS_ATTR((unused)),
		     char	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	если (ls->name) {
		если (ext2fs_dirent_name_len(dirent) != ls->namelen)
			вернуть 0;
		если (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			вернуть 0;
	}
	если (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		если (dirent->inode != ls->inode)
			вернуть 0;
	} иначе {
		если (!dirent->inode)
			вернуть 0;
	}

	если (offset)
		prev->rec_len += dirent->rec_len;
	иначе
		dirent->inode = 0;
	ls->done++;
	вернуть DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const char *name, ext2_ino_t ino,
			целое flags)
{
	errcode_t	retval;
	struct link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	если (!name && !ino)
		вернуть EXT2_ET_INVALID_ARGUMENT;

	если (!(fs->flags & EXT2_FLAG_RW))
		вернуть EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	если (retval)
		вернуть retval;

	вернуть (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

