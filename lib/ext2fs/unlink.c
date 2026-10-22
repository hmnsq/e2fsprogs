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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa link_struct  {
	const var_na	*name;
	ຈຳນວນເຕັມ		namelen;
	ext2_ino_t	inode;
	ຈຳນວນເຕັມ		flags;
	sa_nracanaa ext2_dir_entry *prev;
	ຈຳນວນເຕັມ		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ຈຳນວນເຕັມ unlink_proc(sa_nracanaa ext2_dir_entry *dirent,
		     ຈຳນວນເຕັມ	offset,
		     ຈຳນວນເຕັມ	blocksize EXT2FS_ATTR((unused)),
		     var_na	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ຖ້າ (ls->name) {
		ຖ້າ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ສົ່ງຄືນ 0;
		ຖ້າ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ສົ່ງຄືນ 0;
	}
	ຖ້າ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ຖ້າ (dirent->inode != ls->inode)
			ສົ່ງຄືນ 0;
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (!dirent->inode)
			ສົ່ງຄືນ 0;
	}

	ຖ້າ (offset)
		prev->rec_len += dirent->rec_len;
	ບໍ່ດັ່ງນັ້ນ
		dirent->inode = 0;
	ls->done++;
	ສົ່ງຄືນ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const var_na *name, ext2_ino_t ino,
			ຈຳນວນເຕັມ flags)
{
	errcode_t	retval;
	sa_nracanaa link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ຖ້າ (!name && !ino)
		ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;

	ຖ້າ (!(fs->flags & EXT2_FLAG_RW))
		ສົ່ງຄືນ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	ສົ່ງຄືນ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

