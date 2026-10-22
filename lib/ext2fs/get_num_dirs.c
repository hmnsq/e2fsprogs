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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_انگ_dirs(ext2_filsys fs, ext2_ino_t *ret_انگ_dirs)
{
	dgrp_t	i;
	ext2_ino_t	انگ_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	انگ_dirs = 0;
	max_dirs = fs->super->s_inodes_في_گروهه;
	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		إذا (ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ(fs, i) > max_dirs)
			انگ_dirs += max_dirs / 8;
		وإلا
			انگ_dirs += ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ(fs, i);
	}
	إذا (انگ_dirs > fs->super->s_inodes_ڳڻپ)
		انگ_dirs = fs->super->s_inodes_ڳڻپ;

	*ret_انگ_dirs = انگ_dirs;

	أرجع 0;
}

