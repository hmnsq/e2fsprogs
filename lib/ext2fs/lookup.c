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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct lookup_struct  {
	const char	*name;
	ціле		len;
	ext2_ino_t	*inode;
	ціле		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static ціле lookup_proc(struct ext2_dir_entry *dirent,
		       ціле	offset EXT2FS_ATTR((unused)),
		       ціле	blocksize EXT2FS_ATTR((unused)),
		       char	*buf EXT2FS_ATTR((unused)),
		       void	*priv_data)
{
	struct lookup_struct *ls = (struct lookup_struct *) priv_data;

	якщо (ls->len != ext2fs_dirent_name_len(dirent))
		повернути 0;
	якщо (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		повернути 0;
	*ls->inode = dirent->inode;
	ls->found++;
	повернути DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, const char *name,
			ціле namelen, char *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	struct lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	якщо (retval)
		повернути retval;

	повернути (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


