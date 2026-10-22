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
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa link_struct  {
	const var_na	*name;
	ចំនួនគត់		namelen;
	ext2_ino_t	inode;
	ចំនួនគត់		flags;
	sa_nracanaa ext2_dir_entry *prev;
	ចំនួនគត់		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ចំនួនគត់ unlink_proc(sa_nracanaa ext2_dir_entry *dirent,
		     ចំនួនគត់	offset,
		     ចំនួនគត់	blocksize EXT2FS_ATTR((unused)),
		     var_na	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ប្រសិនបើ (ls->name) {
		ប្រសិនបើ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ត្រឡប់ 0;
		ប្រសិនបើ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ត្រឡប់ 0;
	}
	ប្រសិនបើ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ប្រសិនបើ (dirent->inode != ls->inode)
			ត្រឡប់ 0;
	} បើមិនដូច្នេះ {
		ប្រសិនបើ (!dirent->inode)
			ត្រឡប់ 0;
	}

	ប្រសិនបើ (offset)
		prev->rec_len += dirent->rec_len;
	បើមិនដូច្នេះ
		dirent->inode = 0;
	ls->done++;
	ត្រឡប់ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const var_na *name, ext2_ino_t ino,
			ចំនួនគត់ flags)
{
	errcode_t	retval;
	sa_nracanaa link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ប្រសិនបើ (!name && !ino)
		ត្រឡប់ EXT2_ET_INVALID_ARGUMENT;

	ប្រសិនបើ (!(fs->flags & EXT2_FLAG_RW))
		ត្រឡប់ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	ត្រឡប់ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

