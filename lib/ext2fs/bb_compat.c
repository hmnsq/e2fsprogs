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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, ಪೂರ್ಣಾಙ್ಕ size)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_badblocks_list_create(ret, size);
}

ಶೂನ್ಯ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_badblocks_list_add(bb, blk);
}

ಪೂರ್ಣಾಙ್ಕ badblocks_list_test(badblocks_list bb, blk_t blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_badblocks_list_iterate_begin(bb, ret);
}

ಪೂರ್ಣಾಙ್ಕ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_badblocks_list_iterate(iter, blk);
}

ಶೂನ್ಯ badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
