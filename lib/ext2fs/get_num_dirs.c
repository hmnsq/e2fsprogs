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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_అంకె_dirs(ext2_filsys fs, ext2_ino_t *ret_అంకె_dirs)
{
	dgrp_t	i;
	ext2_ino_t	అంకె_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	అంకె_dirs = 0;
	max_dirs = fs->super->s_inodes_చొప్పున_సమూహం;
	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		ఒకవేళ (ext2fs_bg_వాడిన_dirs_లెక్క(fs, i) > max_dirs)
			అంకె_dirs += max_dirs / 8;
		లేకపోతే
			అంకె_dirs += ext2fs_bg_వాడిన_dirs_లెక్క(fs, i);
	}
	ఒకవేళ (అంకె_dirs > fs->super->s_inodes_లెక్క)
		అంకె_dirs = fs->super->s_inodes_లెక్క;

	*ret_అంకె_dirs = అంకె_dirs;

	తిరిగి 0;
}

