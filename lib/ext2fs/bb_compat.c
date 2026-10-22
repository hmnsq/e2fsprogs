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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, পূর্ণাঙ্ক size)
{
	ঘূৰাই ext2fs_badblocks_list_create(ret, size);
}

শূন্য badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	ঘূৰাই ext2fs_badblocks_list_add(bb, blk);
}

পূর্ণাঙ্ক badblocks_list_test(badblocks_list bb, blk_t blk)
{
	ঘূৰাই ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	ঘূৰাই ext2fs_badblocks_list_iterate_begin(bb, ret);
}

পূর্ণাঙ্ক badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	ঘূৰাই ext2fs_badblocks_list_iterate(iter, blk);
}

শূন্য badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
