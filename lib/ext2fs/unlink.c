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
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct link_struct  {
	const char	*name;
	цяло		namelen;
	ext2_ino_t	inode;
	цяло		flags;
	struct ext2_dir_entry *prev;
	цяло		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static цяло unlink_proc(struct ext2_dir_entry *dirent,
		     цяло	offset,
		     цяло	blocksize EXT2FS_ATTR((unused)),
		     char	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ако (ls->name) {
		ако (ext2fs_dirent_name_len(dirent) != ls->namelen)
			върни 0;
		ако (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			върни 0;
	}
	ако (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ако (dirent->inode != ls->inode)
			върни 0;
	} иначе {
		ако (!dirent->inode)
			върни 0;
	}

	ако (offset)
		prev->rec_len += dirent->rec_len;
	иначе
		dirent->inode = 0;
	ls->done++;
	върни DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const char *name, ext2_ino_t ino,
			цяло flags)
{
	errcode_t	retval;
	struct link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ако (!name && !ino)
		върни EXT2_ET_INVALID_ARGUMENT;

	ако (!(fs->flags & EXT2_FLAG_RW))
		върни EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ако (retval)
		върни retval;

	върни (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

