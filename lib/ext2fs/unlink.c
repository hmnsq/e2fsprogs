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
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct link_struct  {
	const char	*name;
	整数		namelen;
	ext2_ino_t	inode;
	整数		flags;
	struct ext2_dir_entry *prev;
	整数		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static 整数 unlink_proc(struct ext2_dir_entry *dirent,
		     整数	offset,
		     整数	blocksize EXT2FS_ATTR((unused)),
		     char	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	如果 (ls->name) {
		如果 (ext2fs_dirent_name_len(dirent) != ls->namelen)
			返回 0;
		如果 (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			返回 0;
	}
	如果 (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		如果 (dirent->inode != ls->inode)
			返回 0;
	} 否则 {
		如果 (!dirent->inode)
			返回 0;
	}

	如果 (offset)
		prev->rec_len += dirent->rec_len;
	否则
		dirent->inode = 0;
	ls->done++;
	返回 DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const char *name, ext2_ino_t ino,
			整数 flags)
{
	errcode_t	retval;
	struct link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	如果 (!name && !ino)
		返回 EXT2_ET_INVALID_ARGUMENT;

	如果 (!(fs->flags & EXT2_FLAG_RW))
		返回 EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	如果 (retval)
		返回 retval;

	返回 (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

