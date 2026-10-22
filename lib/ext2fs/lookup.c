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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਣਤਰ lookup_struct  {
	ਅਚਲ ਅੱਖਰ	*name;
	ਪੂਰਨ		len;
	ext2_ino_t	*inode;
	ਪੂਰਨ		ਮਿਲਿਆ;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ lookup_proc(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
		       ਪੂਰਨ	ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		       ਪੂਰਨ	blocksize EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		       ਅੱਖਰ	*buf EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		       ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ lookup_struct *ls = (ਬਣਤਰ lookup_struct *) priv_ਅੰਕੜੇ;

	ਜੇ (ls->len != ext2fs_dirent_name_len(dirent))
		ਵਾਪਸ 0;
	ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ਵਾਪਸ 0;
	*ls->inode = dirent->inode;
	ls->ਮਿਲਿਆ++;
	ਵਾਪਸ DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ਅਚਲ ਅੱਖਰ *name,
			ਪੂਰਨ namelen, ਅੱਖਰ *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ਬਣਤਰ lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.ਮਿਲਿਆ = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ (ls.ਮਿਲਿਆ) ? 0 : EXT2_ET_FILE_NOT_ਮਿਲਿਆ;
}


