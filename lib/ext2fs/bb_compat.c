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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, ਪੂਰਨ ਮਾਪ)
{
	ਵਾਪਸ ext2fs_badblocks_list_create(ret, ਮਾਪ);
}

ਖਾਲੀ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_ਜੋੜੋ(badblocks_list bb, blk_t blk)
{
	ਵਾਪਸ ext2fs_badblocks_list_ਜੋੜੋ(bb, blk);
}

ਪੂਰਨ badblocks_list_test(badblocks_list bb, blk_t blk)
{
	ਵਾਪਸ ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	ਵਾਪਸ ext2fs_badblocks_list_iterate_begin(bb, ret);
}

ਪੂਰਨ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	ਵਾਪਸ ext2fs_badblocks_list_iterate(iter, blk);
}

ਖਾਲੀ badblocks_list_iterate_ਸਿਰਾ(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_ਸਿਰਾ(iter);
}
