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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

errcode_t ext2fs_write_bb_FILE(ext2_badblocks_list bb_list,
			       unsigned முழு flags EXT2FS_ATTR((unused)),
			       FILE *f)
{
	badblocks_iterate	bb_iter;
	blk_t			blk;
	errcode_t		retval;

	retval = ext2fs_badblocks_list_iterate_begin(bb_list, &bb_iter);
	எனில் (retval)
		திரும்பு retval;

	போது (ext2fs_badblocks_list_iterate(bb_iter, &blk)) {
		fprintf(f, "%u\n", blk);
	}
	ext2fs_badblocks_list_iterate_end(bb_iter);
	திரும்பு 0;
}
