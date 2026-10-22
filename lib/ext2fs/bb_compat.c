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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#אם HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#אם HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, صحيح size)
{
	החזר ext2fs_badblocks_list_create(ret, size);
}

فراغ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	החזר ext2fs_badblocks_list_add(bb, blk);
}

صحيح badblocks_list_test(badblocks_list bb, blk_t blk)
{
	החזר ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	החזר ext2fs_badblocks_list_iterate_begin(bb, ret);
}

صحيح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	החזר ext2fs_badblocks_list_iterate(iter, blk);
}

فراغ badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
