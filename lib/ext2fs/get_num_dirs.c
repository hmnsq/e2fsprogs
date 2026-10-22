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
errcode_t ext2fs_get_ژمارە_dirs(ext2_filsys fs, ext2_ino_t *ret_ژمارە_dirs)
{
	dgrp_t	i;
	ext2_ino_t	ژمارە_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ژمارە_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	لكل (i = 0; i < fs->group_desc_ژماردن; i++) {
		إذا (ext2fs_bg_used_dirs_ژماردن(fs, i) > max_dirs)
			ژمارە_dirs += max_dirs / 8;
		وإلا
			ژمارە_dirs += ext2fs_bg_used_dirs_ژماردن(fs, i);
	}
	إذا (ژمارە_dirs > fs->super->s_inodes_ژماردن)
		ژمارە_dirs = fs->super->s_inodes_ژماردن;

	*ret_ژمارە_dirs = ژمارە_dirs;

	أرجع 0;
}

