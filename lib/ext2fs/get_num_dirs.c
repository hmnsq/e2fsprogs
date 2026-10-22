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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_ਅੰਕ_dirs(ext2_filsys fs, ext2_ino_t *ret_ਅੰਕ_dirs)
{
	dgrp_t	i;
	ext2_ino_t	ਅੰਕ_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਅੰਕ_dirs = 0;
	max_dirs = fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ;
	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਜੇ (ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ(fs, i) > max_dirs)
			ਅੰਕ_dirs += max_dirs / 8;
		ਨਹੀਂਤਾਂ
			ਅੰਕ_dirs += ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ(fs, i);
	}
	ਜੇ (ਅੰਕ_dirs > fs->super->s_inodes_ਗਿਣਤੀ)
		ਅੰਕ_dirs = fs->super->s_inodes_ਗਿਣਤੀ;

	*ret_ਅੰਕ_dirs = ਅੰਕ_dirs;

	ਵਾਪਸ 0;
}

