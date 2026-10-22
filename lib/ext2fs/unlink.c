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
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa link_struct  {
	const var_na	*name;
	පූර්ණ_සංඛ්‍යා		namelen;
	ext2_ino_t	inode;
	පූර්ණ_සංඛ්‍යා		flags;
	sa_nracanaa ext2_dir_entry *prev;
	පූර්ණ_සංඛ්‍යා		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static පූර්ණ_සංඛ්‍යා unlink_proc(sa_nracanaa ext2_dir_entry *dirent,
		     පූර්ණ_සංඛ්‍යා	offset,
		     පූර්ණ_සංඛ්‍යා	blocksize EXT2FS_ATTR((unused)),
		     var_na	*buf EXT2FS_ATTR((unused)),
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	නම් (ls->name) {
		නම් (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ආපසු 0;
		නම් (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ආපසු 0;
	}
	නම් (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		නම් (dirent->inode != ls->inode)
			ආපසු 0;
	} නැතහොත් {
		නම් (!dirent->inode)
			ආපසු 0;
	}

	නම් (offset)
		prev->rec_len += dirent->rec_len;
	නැතහොත්
		dirent->inode = 0;
	ls->done++;
	ආපසු DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			const var_na *name, ext2_ino_t ino,
			පූර්ණ_සංඛ්‍යා flags)
{
	errcode_t	retval;
	sa_nracanaa link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	නම් (!name && !ino)
		ආපසු EXT2_ET_INVALID_ARGUMENT;

	නම් (!(fs->flags & EXT2_FLAG_RW))
		ආපසු EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	නම් (retval)
		ආපසු retval;

	ආපසු (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

