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

#include "config.h"
#include <stdio.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_नुमेरो_dirs(ext2_filsys fs, ext2_ino_t *ret_नुमेरो_dirs)
{
	dgrp_t	i;
	ext2_ino_t	नुमेरो_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	नुमेरो_dirs = 0;
	max_dirs = fs->super->s_inodes_per_group;
	for (i = 0; i < fs->group_desc_बिलांग; i++) {
		if (ext2fs_bg_used_dirs_बिलांग(fs, i) > max_dirs)
			नुमेरो_dirs += max_dirs / 8;
		else
			नुमेरो_dirs += ext2fs_bg_used_dirs_बिलांग(fs, i);
	}
	if (नुमेरो_dirs > fs->super->s_inodes_बिलांग)
		नुमेरो_dirs = fs->super->s_inodes_बिलांग;

	*ret_नुमेरो_dirs = नुमेरो_dirs;

	return 0;
}

