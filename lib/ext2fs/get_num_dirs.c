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

#include "config.h"
#include <stdio.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

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
	cho (i = 0; i < fs->group_desc_count; i++) {
		nếu (ext2fs_bg_used_dirs_count(fs, i) > max_dirs)
			num_dirs += max_dirs / 8;
		không_thì
			num_dirs += ext2fs_bg_used_dirs_count(fs, i);
	}
	nếu (num_dirs > fs->super->s_inodes_count)
		num_dirs = fs->super->s_inodes_count;

	*ret_num_dirs = num_dirs;

	trả_về 0;
}

