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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, પૂર્ણાંક size)
{
	પાછા ext2fs_badblocks_list_create(ret, size);
}

શૂન્ય badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	પાછા ext2fs_badblocks_list_add(bb, blk);
}

પૂર્ણાંક badblocks_list_test(badblocks_list bb, blk_t blk)
{
	પાછા ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	પાછા ext2fs_badblocks_list_iterate_begin(bb, ret);
}

પૂર્ણાંક badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	પાછા ext2fs_badblocks_list_iterate(iter, blk);
}

શૂન્ય badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
