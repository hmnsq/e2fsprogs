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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, పూర్ణాంకం కొలత)
{
	తిరిగి ext2fs_badblocks_list_create(ret, కొలత);
}

శూన్య badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_కూడు(badblocks_list bb, blk_t blk)
{
	తిరిగి ext2fs_badblocks_list_కూడు(bb, blk);
}

పూర్ణాంకం badblocks_list_test(badblocks_list bb, blk_t blk)
{
	తిరిగి ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	తిరిగి ext2fs_badblocks_list_iterate_begin(bb, ret);
}

పూర్ణాంకం badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	తిరిగి ext2fs_badblocks_list_iterate(iter, blk);
}

శూన్య badblocks_list_iterate_చివర(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_చివర(iter);
}
