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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, പൂർണ്ണസംഖ്യ size)
{
	തിരികെ ext2fs_badblocks_list_create(ret, size);
}

ശൂന്യം badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	തിരികെ ext2fs_badblocks_list_add(bb, blk);
}

പൂർണ്ണസംഖ്യ badblocks_list_test(badblocks_list bb, blk_t blk)
{
	തിരികെ ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	തിരികെ ext2fs_badblocks_list_iterate_begin(bb, ret);
}

പൂർണ്ണസംഖ്യ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	തിരികെ ext2fs_badblocks_list_iterate(iter, blk);
}

ശൂന്യം badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
