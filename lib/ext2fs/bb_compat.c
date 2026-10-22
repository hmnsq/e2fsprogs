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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, صحيح اندازه)
{
	أرجع ext2fs_badblocks_list_create(ret, اندازه);
}

فراغ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_جمع_کړه(badblocks_list bb, blk_t blk)
{
	أرجع ext2fs_badblocks_list_جمع_کړه(bb, blk);
}

صحيح badblocks_list_test(badblocks_list bb, blk_t blk)
{
	أرجع ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	أرجع ext2fs_badblocks_list_iterate_begin(bb, ret);
}

صحيح badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	أرجع ext2fs_badblocks_list_iterate(iter, blk);
}

فراغ badblocks_list_iterate_پای_ټکی(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_پای_ټکی(iter);
}
