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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਣਤਰ link_struct  {
	ਅਚਲ ਅੱਖਰ	*name;
	ਪੂਰਨ		namelen;
	ext2_ino_t	inode;
	ਪੂਰਨ		flags;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *prev;
	ਪੂਰਨ		ਪੂਰਾ;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ unlink_proc(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
		     ਪੂਰਨ	ਵਿੱਥ,
		     ਪੂਰਨ	blocksize EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		     ਅੱਖਰ	*buf EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		     ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ link_struct *ls = (ਬਣਤਰ link_struct *) priv_ਅੰਕੜੇ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ਜੇ (ls->name) {
		ਜੇ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ਵਾਪਸ 0;
		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ਵਾਪਸ 0;
	}
	ਜੇ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ਜੇ (dirent->inode != ls->inode)
			ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!dirent->inode)
			ਵਾਪਸ 0;
	}

	ਜੇ (ਵਿੱਥ)
		prev->rec_len += dirent->rec_len;
	ਨਹੀਂਤਾਂ
		dirent->inode = 0;
	ls->ਪੂਰਾ++;
	ਵਾਪਸ DIRENT_ABORT|DIRENT_ਬਦਲਿਆ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ਅਚਲ ਅੱਖਰ *name, ext2_ino_t ino,
			ਪੂਰਨ flags)
{
	errcode_t	retval;
	ਬਣਤਰ link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!name && !ino)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? ਲੜੀ_ਲੰਬਾਈ(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.ਪੂਰਾ = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ,
				    0, unlink_proc, &ls);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ (ls.ਪੂਰਾ) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

