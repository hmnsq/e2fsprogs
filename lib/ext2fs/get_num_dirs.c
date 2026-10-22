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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_অঙ্ক_dirs(ext2_filsys fs, ext2_ino_t *ret_অঙ্ক_dirs)
{
	dgrp_t	i;
	ext2_ino_t	অঙ্ক_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	অঙ্ক_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	জন্য (i = 0; i < fs->group_desc_গুনতি; i++) {
		যদি (ext2fs_bg_used_dirs_গুনতি(fs, i) > max_dirs)
			অঙ্ক_dirs += max_dirs / 8;
		অন্যথা
			অঙ্ক_dirs += ext2fs_bg_used_dirs_গুনতি(fs, i);
	}
	যদি (অঙ্ক_dirs > fs->super->s_inodes_গুনতি)
		অঙ্ক_dirs = fs->super->s_inodes_গুনতি;

	*ret_অঙ্ক_dirs = অঙ্ক_dirs;

	ফেরত 0;
}

