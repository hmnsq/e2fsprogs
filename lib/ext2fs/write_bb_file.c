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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			       FILE *f)
{
	badblocks_iterate	bb_iter;
	blk_t			blk;
	errcode_t		retval;

	retval = ext2fs_badblocks_list_iterate_begin(bb_list, &bb_iter);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜਦੋਂ (ext2fs_badblocks_list_iterate(bb_iter, &blk)) {
		fprintf(f, "%u\n", blk);
	}
	ext2fs_badblocks_list_iterate_ਸਿਰਾ(bb_iter);
	ਵਾਪਸ 0;
}
