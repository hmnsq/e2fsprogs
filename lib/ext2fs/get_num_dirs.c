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
#پایان_اگر
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

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
	max_dirs = fs->super->s_inodes_در_هر_گروه;
	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		إذا (ext2fs_bg_استفاده_شده_dirs_شمار(fs, i) > max_dirs)
			رقم_dirs += max_dirs / 8;
		وإلا
			رقم_dirs += ext2fs_bg_استفاده_شده_dirs_شمار(fs, i);
	}
	إذا (رقم_dirs > fs->super->s_inodes_شمار)
		رقم_dirs = fs->super->s_inodes_شمار;

	*ret_رقم_dirs = رقم_dirs;

	أرجع 0;
}

