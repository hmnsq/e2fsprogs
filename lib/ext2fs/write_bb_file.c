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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
			       గుర్తులేని పూర్ణాంకం flags EXT2FS_ATTR((వాడని)),
			       FILE *f)
{
	badblocks_iterate	bb_iter;
	blk_t			blk;
	errcode_t		retval;

	retval = ext2fs_badblocks_list_iterate_begin(bb_list, &bb_iter);
	ఒకవేళ (retval)
		తిరిగి retval;

	అయితే (ext2fs_badblocks_list_iterate(bb_iter, &blk)) {
		fprintf(f, "%u\n", blk);
	}
	ext2fs_badblocks_list_iterate_చివర(bb_iter);
	తిరిగి 0;
}
