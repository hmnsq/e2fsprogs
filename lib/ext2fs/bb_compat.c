/*
 * bb_compat.c --- compatibility badblocks routines
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, صحیح size)
{
	بازگشت ext2fs_badblocks_list_create(ret, size);
}

تهی badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	بازگشت ext2fs_badblocks_list_add(bb, blk);
}

صحیح badblocks_list_test(badblocks_list bb, blk_t blk)
{
	بازگشت ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	بازگشت ext2fs_badblocks_list_iterate_begin(bb, ret);
}

صحیح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	بازگشت ext2fs_badblocks_list_iterate(iter, blk);
}

تهی badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
