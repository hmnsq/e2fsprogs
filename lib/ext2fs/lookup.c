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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸಂರಚನಾ lookup_struct  {
	ಧ್ರುವ ವರ್ಣ	*name;
	ಪೂರ್ಣಾಙ್ಕ		len;
	ext2_ino_t	*inode;
	ಪೂರ್ಣಾಙ್ಕ		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ lookup_proc(ಸಂರಚನಾ ext2_dir_entry *dirent,
		       ಪೂರ್ಣಾಙ್ಕ	offset EXT2FS_ATTR((unused)),
		       ಪೂರ್ಣಾಙ್ಕ	blocksize EXT2FS_ATTR((unused)),
		       ವರ್ಣ	*buf EXT2FS_ATTR((unused)),
		       ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ lookup_struct *ls = (ಸಂರಚನಾ lookup_struct *) priv_data;

	ಯದಿ (ls->len != ext2fs_dirent_name_len(dirent))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ಪ್ರತ್ಯಾಗಮ 0;
	*ls->inode = dirent->inode;
	ls->found++;
	ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ಧ್ರುವ ವರ್ಣ *name,
			ಪೂರ್ಣಾಙ್ಕ namelen, ವರ್ಣ *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ಸಂರಚನಾ lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


