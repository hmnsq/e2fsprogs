/*
 * lookup.c --- ext2fs directory lookup operations
 *
 * Copyright (C) 1993, 1994, 1994, 1995 Theodore Ts'o.
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

sa_nracanaa lookup_struct  {
	const var_na	*name;
	ཧྲིལ		len;
	ext2_ino_t	*inode;
	ཧྲིལ		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ཧྲིལ lookup_proc(sa_nracanaa ext2_dir_entry *dirent,
		       ཧྲིལ	offset EXT2FS_ATTR((unused)),
		       ཧྲིལ	blocksize EXT2FS_ATTR((unused)),
		       var_na	*buf EXT2FS_ATTR((unused)),
		       void	*priv_data)
{
	sa_nracanaa lookup_struct *ls = (sa_nracanaa lookup_struct *) priv_data;

	གལ་ཏེ (ls->len != ext2fs_dirent_name_len(dirent))
		ཕྱིར 0;
	གལ་ཏེ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ཕྱིར 0;
	*ls->inode = dirent->inode;
	ls->found++;
	ཕྱིར DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, const var_na *name,
			ཧྲིལ namelen, var_na *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	sa_nracanaa lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	ཕྱིར (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


