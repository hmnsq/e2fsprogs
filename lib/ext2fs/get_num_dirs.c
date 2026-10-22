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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_num_dirs(ext2_filsys fs, ext2_ino_t *ret_num_dirs)
{
	dgrp_t	i;
	ext2_ino_t	num_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	num_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	لكل (i = 0; i < fs->group_desc_count; i++) {
		ܐܢ (ext2fs_bg_used_dirs_count(fs, i) > max_dirs)
			num_dirs += max_dirs / 8;
		ܐܚܪܢܐ
			num_dirs += ext2fs_bg_used_dirs_count(fs, i);
	}
	ܐܢ (num_dirs > fs->super->s_inodes_count)
		num_dirs = fs->super->s_inodes_count;

	*ret_num_dirs = num_dirs;

	ܦܢܐ 0;
}

