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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, पूर्णांक आकार)
{
	वापस ext2fs_badblocks_list_create(ret, आकार);
}

व्योम badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	वापस ext2fs_badblocks_list_add(bb, blk);
}

पूर्णांक badblocks_list_test(badblocks_list bb, blk_t blk)
{
	वापस ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	वापस ext2fs_badblocks_list_iterate_begin(bb, ret);
}

पूर्णांक badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	वापस ext2fs_badblocks_list_iterate(iter, blk);
}

व्योम badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
