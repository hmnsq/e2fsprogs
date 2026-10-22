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
#ᰀᰧ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa link_struct  {
	const var_na	*name;
	puur_naa_nka		namelen;
	ext2_ino_t	inode;
	puur_naa_nka		flags;
	sa_nracanaa ext2_dir_entry *prev;
	puur_naa_nka		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static puur_naa_nka unlink_proc(sa_nracanaa ext2_dir_entry *dirent,
		     puur_naa_nka	offset,
		     puur_naa_nka	blocksize EXT2FS_ATTR((unused)),
		     var_na	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ᰀᰧ (ls->name) {
		ᰀᰧ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ᰀᰧᰌ 0;
		ᰀᰧ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ᰀᰧᰌ 0;
	}
	ᰀᰧ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ᰀᰧ (dirent->inode != ls->inode)
			ᰀᰧᰌ 0;
	} ᰀᰧᰀ {
		ᰀᰧ (!dirent->inode)
			ᰀᰧᰌ 0;
	}

	ᰀᰧ (offset)
		prev->rec_len += dirent->rec_len;
	ᰀᰧᰀ
		dirent->inode = 0;
	ls->done++;
	ᰀᰧᰌ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const var_na *name, ext2_ino_t ino,
			puur_naa_nka flags)
{
	errcode_t	retval;
	sa_nracanaa link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᰀᰧ (!name && !ino)
		ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

	ᰀᰧ (!(fs->flags & EXT2_FLAG_RW))
		ᰀᰧᰌ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	ᰀᰧᰌ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

