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
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_رقم_dirs(ext2_filsys fs, ext2_ino_t *ret_رقم_dirs)
{
	dgrp_t	i;
	ext2_ino_t	رقم_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	رقم_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	برای (i = 0; i < fs->group_desc_شمار; i++) {
		اگر (ext2fs_bg_used_dirs_شمار(fs, i) > max_dirs)
			رقم_dirs += max_dirs / 8;
		وگرنه
			رقم_dirs += ext2fs_bg_used_dirs_شمار(fs, i);
	}
	اگر (رقم_dirs > fs->super->s_inodes_شمار)
		رقم_dirs = fs->super->s_inodes_شمار;

	*ret_رقم_dirs = رقم_dirs;

	بازگشت 0;
}

