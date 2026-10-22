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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

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
	max_dirs = fs->super->s_inodes_فی_گروہ;
	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		اگر (ext2fs_bg_مستعمل_dirs_گنتی(fs, i) > max_dirs)
			عدد_dirs += max_dirs / 8;
		ورنہ
			عدد_dirs += ext2fs_bg_مستعمل_dirs_گنتی(fs, i);
	}
	اگر (عدد_dirs > fs->super->s_inodes_گنتی)
		عدد_dirs = fs->super->s_inodes_گنتی;

	*ret_عدد_dirs = عدد_dirs;

	واپس 0;
}

