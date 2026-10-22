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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_അക്കം_dirs(ext2_filsys fs, ext2_ino_t *ret_അക്കം_dirs)
{
	dgrp_t	i;
	ext2_ino_t	അക്കം_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	അക്കം_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	വേണ്ടി (i = 0; i < fs->group_desc_എണ്ണം; i++) {
		എങ്കിൽ (ext2fs_bg_used_dirs_എണ്ണം(fs, i) > max_dirs)
			അക്കം_dirs += max_dirs / 8;
		അല്ലെങ്കിൽ
			അക്കം_dirs += ext2fs_bg_used_dirs_എണ്ണം(fs, i);
	}
	എങ്കിൽ (അക്കം_dirs > fs->super->s_inodes_എണ്ണം)
		അക്കം_dirs = fs->super->s_inodes_എണ്ണം;

	*ret_അക്കം_dirs = അക്കം_dirs;

	തിരികെ 0;
}

