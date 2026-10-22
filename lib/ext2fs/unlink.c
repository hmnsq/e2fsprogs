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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸಂರಚನಾ link_struct  {
	ಧ್ರುವ ವರ್ಣ	*name;
	ಪೂರ್ಣಾಙ್ಕ		namelen;
	ext2_ino_t	inode;
	ಪೂರ್ಣಾಙ್ಕ		flags;
	ಸಂರಚನಾ ext2_dir_entry *prev;
	ಪೂರ್ಣಾಙ್ಕ		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ unlink_proc(ಸಂರಚನಾ ext2_dir_entry *dirent,
		     ಪೂರ್ಣಾಙ್ಕ	offset,
		     ಪೂರ್ಣಾಙ್ಕ	blocksize EXT2FS_ATTR((unused)),
		     ವರ್ಣ	*buf EXT2FS_ATTR((unused)),
		     ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ link_struct *ls = (ಸಂರಚನಾ link_struct *) priv_data;
	ಸಂರಚನಾ ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ಯದಿ (ls->name) {
		ಯದಿ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ಯದಿ (dirent->inode != ls->inode)
			ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ {
		ಯದಿ (!dirent->inode)
			ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (offset)
		prev->rec_len += dirent->rec_len;
	ಅನ್ಯಥಾ
		dirent->inode = 0;
	ls->done++;
	ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t ino,
			ಪೂರ್ಣಾಙ್ಕ flags)
{
	errcode_t	retval;
	ಸಂರಚನಾ link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!name && !ino)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (!(fs->flags & EXT2_FLAG_RW))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

