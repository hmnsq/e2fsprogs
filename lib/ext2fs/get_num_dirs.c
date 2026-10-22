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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_ଅଙ୍କ_dirs(ext2_filsys fs, ext2_ino_t *ret_ଅଙ୍କ_dirs)
{
	dgrp_t	i;
	ext2_ino_t	ଅଙ୍କ_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଅଙ୍କ_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++) {
		ଯଦି (ext2fs_bg_used_dirs_ଗଣନା(fs, i) > max_dirs)
			ଅଙ୍କ_dirs += max_dirs / 8;
		ଅନ୍ୟଥା
			ଅଙ୍କ_dirs += ext2fs_bg_used_dirs_ଗଣନା(fs, i);
	}
	ଯଦି (ଅଙ୍କ_dirs > fs->super->s_inodes_ଗଣନା)
		ଅଙ୍କ_dirs = fs->super->s_inodes_ଗଣନା;

	*ret_ଅଙ୍କ_dirs = ଅଙ୍କ_dirs;

	ଫେରନ୍ତୁ 0;
}

