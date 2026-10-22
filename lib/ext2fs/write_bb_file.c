/*
 * write_bb_file.c --- write a list of bad blocks to a FILE *
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)),
			       FILE *f)
{
	badblocks_iterate	bb_iter;
	blk_t			blk;
	errcode_t		retval;

	retval = ext2fs_badblocks_list_iterate_begin(bb_list, &bb_iter);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆಗ (ext2fs_badblocks_list_iterate(bb_iter, &blk)) {
		fprintf(f, "%u\n", blk);
	}
	ext2fs_badblocks_list_iterate_ಕೊನೆ(bb_iter);
	ಹಿಂತಿರುಗಿ 0;
}
