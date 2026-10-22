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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ link_struct  {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*name;
	ಪೂರ್ಣಾಂಕ		namelen;
	ext2_ino_t	inode;
	ಪೂರ್ಣಾಂಕ		flags;
	ರಚನೆ ext2_dir_ನಮೂದು *prev;
	ಪೂರ್ಣಾಂಕ		ಮುಗಿದ;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ unlink_proc(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
		     ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
		     ಪೂರ್ಣಾಂಕ	blocksize EXT2FS_ATTR((ಬಳಸದ)),
		     ಅಕ್ಷರ	*buf EXT2FS_ATTR((ಬಳಸದ)),
		     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ link_struct *ls = (ರಚನೆ link_struct *) priv_ದತ್ತಾಂಶ;
	ರಚನೆ ext2_dir_ನಮೂದು *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ಆದರೆ (ls->name) {
		ಆದರೆ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ಆದರೆ (dirent->inode != ls->inode)
			ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!dirent->inode)
			ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (ಸ್ಥಾನಾಂತರ)
		prev->rec_len += dirent->rec_len;
	ಇಲ್ಲದಿದ್ದರೆ
		dirent->inode = 0;
	ls->ಮುಗಿದ++;
	ಹಿಂತಿರುಗಿ DIRENT_ABORT|DIRENT_ಬದಲಾಯಿತು;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t ino,
			ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t	retval;
	ರಚನೆ link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!name && !ino)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.ಮುಗಿದ = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು,
				    0, unlink_proc, &ls);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ (ls.ಮುಗಿದ) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

