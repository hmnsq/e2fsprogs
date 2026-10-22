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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_अंक_dirs(ext2_filsys fs, ext2_ino_t *ret_अंक_dirs)
{
	dgrp_t	i;
	ext2_ino_t	अंक_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अंक_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	क्रम (i = 0; i < fs->group_desc_गिनती; i++) {
		अगर (ext2fs_bg_used_dirs_गिनती(fs, i) > max_dirs)
			अंक_dirs += max_dirs / 8;
		अन्यथा
			अंक_dirs += ext2fs_bg_used_dirs_गिनती(fs, i);
	}
	अगर (अंक_dirs > fs->super->s_inodes_गिनती)
		अंक_dirs = fs->super->s_inodes_गिनती;

	*ret_अंक_dirs = अंक_dirs;

	वापस 0;
}

