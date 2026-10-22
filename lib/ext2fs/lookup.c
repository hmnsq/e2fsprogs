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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ lookup_struct  {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*name;
	ಪೂರ್ಣಾಂಕ		len;
	ext2_ino_t	*inode;
	ಪೂರ್ಣಾಂಕ		ಸಿಕ್ಕಿತು;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ lookup_proc(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
		       ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
		       ಪೂರ್ಣಾಂಕ	blocksize EXT2FS_ATTR((ಬಳಸದ)),
		       ಅಕ್ಷರ	*buf EXT2FS_ATTR((ಬಳಸದ)),
		       ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ lookup_struct *ls = (ರಚನೆ lookup_struct *) priv_ದತ್ತಾಂಶ;

	ಆದರೆ (ls->len != ext2fs_dirent_name_len(dirent))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ಹಿಂತಿರುಗಿ 0;
	*ls->inode = dirent->inode;
	ls->ಸಿಕ್ಕಿತು++;
	ಹಿಂತಿರುಗಿ DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			ಪೂರ್ಣಾಂಕ namelen, ಅಕ್ಷರ *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ರಚನೆ lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.ಸಿಕ್ಕಿತು = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ (ls.ಸಿಕ್ಕಿತು) ? 0 : EXT2_ET_FILE_NOT_ಸಿಕ್ಕಿತು;
}


