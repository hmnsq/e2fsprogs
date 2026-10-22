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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * Returns the number of directories in the filesystem as reported by
 * the group descriptors.  Of course, the group descriptors could be
 * wrong!
 */
errcode_t ext2fs_get_अङ्कः_dirs(ext2_filsys fs, ext2_ino_t *ret_अङ्कः_dirs)
{
	dgrp_t	i;
	ext2_ino_t	अङ्कः_dirs, max_dirs;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अङ्कः_dirs = 0;
	max_dirs = fs->super->s_inodes_प्रत्येकम्_गणः;
	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		यदि (ext2fs_bg_प्रयुक्तम्_dirs_गणनम्(fs, i) > max_dirs)
			अङ्कः_dirs += max_dirs / 8;
		अन्यथा
			अङ्कः_dirs += ext2fs_bg_प्रयुक्तम्_dirs_गणनम्(fs, i);
	}
	यदि (अङ्कः_dirs > fs->super->s_inodes_गणनम्)
		अङ्कः_dirs = fs->super->s_inodes_गणनम्;

	*ret_अङ्कः_dirs = अङ्कः_dirs;

	प्रत्यागम 0;
}

