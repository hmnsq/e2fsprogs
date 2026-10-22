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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_અંક_dirs(ext2_filsys fs, ext2_ino_t *ret_અંક_dirs)
{
	dgrp_t	i;
	ext2_ino_t	અંક_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	અંક_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	માટે (i = 0; i < fs->group_desc_ગણતરી; i++) {
		જો (ext2fs_bg_used_dirs_ગણતરી(fs, i) > max_dirs)
			અંક_dirs += max_dirs / 8;
		નહીંતર
			અંક_dirs += ext2fs_bg_used_dirs_ગણતરી(fs, i);
	}
	જો (અંક_dirs > fs->super->s_inodes_ગણતરી)
		અંક_dirs = fs->super->s_inodes_ગણતરી;

	*ret_અંક_dirs = અંક_dirs;

	પાછા 0;
}

