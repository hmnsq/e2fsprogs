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
#د_که_پای
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_عدد_dirs(ext2_filsys fs, ext2_ino_t *ret_عدد_dirs)
{
	dgrp_t	i;
	ext2_ino_t	عدد_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	عدد_dirs = 0;
	max_dirs = fs->super->s_inodes_فی_ډله;
	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		إذا (ext2fs_bg_کارول_شوی_dirs_شمېر(fs, i) > max_dirs)
			عدد_dirs += max_dirs / 8;
		وإلا
			عدد_dirs += ext2fs_bg_کارول_شوی_dirs_شمېر(fs, i);
	}
	إذا (عدد_dirs > fs->super->s_inodes_شمېر)
		عدد_dirs = fs->super->s_inodes_شمېر;

	*ret_عدد_dirs = عدد_dirs;

	أرجع 0;
}

