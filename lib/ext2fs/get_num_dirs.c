/*
 * get_num_dirs.c -- calculate number of directories
 *
 * Copyright 1997 by Theodore Ts'o
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_ಅಂಕೆ_dirs(ext2_filsys fs, ext2_ino_t *ret_ಅಂಕೆ_dirs)
{
	dgrp_t	i;
	ext2_ino_t	ಅಂಕೆ_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಅಂಕೆ_dirs = 0;
	max_dirs = fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು;
	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಆದರೆ (ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ(fs, i) > max_dirs)
			ಅಂಕೆ_dirs += max_dirs / 8;
		ಇಲ್ಲದಿದ್ದರೆ
			ಅಂಕೆ_dirs += ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ(fs, i);
	}
	ಆದರೆ (ಅಂಕೆ_dirs > fs->super->s_inodes_ಎಣಿಕೆ)
		ಅಂಕೆ_dirs = fs->super->s_inodes_ಎಣಿಕೆ;

	*ret_ಅಂಕೆ_dirs = ಅಂಕೆ_dirs;

	ಹಿಂತಿರುಗಿ 0;
}

