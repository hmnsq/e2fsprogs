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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_இலக்கம்_dirs(ext2_filsys fs, ext2_ino_t *ret_இலக்கம்_dirs)
{
	dgrp_t	i;
	ext2_ino_t	இலக்கம்_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	இலக்கம்_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		எனில் (ext2fs_bg_used_dirs_மொத்தம்(fs, i) > max_dirs)
			இலக்கம்_dirs += max_dirs / 8;
		இல்லையெனில்
			இலக்கம்_dirs += ext2fs_bg_used_dirs_மொத்தம்(fs, i);
	}
	எனில் (இலக்கம்_dirs > fs->super->s_inodes_மொத்தம்)
		இலக்கம்_dirs = fs->super->s_inodes_மொத்தம்;

	*ret_இலக்கம்_dirs = இலக்கம்_dirs;

	திரும்பு 0;
}

