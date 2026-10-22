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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ଫେରନ୍ତୁ ext2fs_badblocks_list_create(ret, size);
}

ଶୂନ୍ୟ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	ଫେରନ୍ତୁ ext2fs_badblocks_list_add(bb, blk);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା badblocks_list_test(badblocks_list bb, blk_t blk)
{
	ଫେରନ୍ତୁ ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	ଫେରନ୍ତୁ ext2fs_badblocks_list_iterate_begin(bb, ret);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	ଫେରନ୍ତୁ ext2fs_badblocks_list_iterate(iter, blk);
}

ଶୂନ୍ୟ badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
