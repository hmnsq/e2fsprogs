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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಹಿಂತಿರುಗಿ ext2fs_badblocks_list_create(ret, ಗಾತ್ರ_2);
}

ಶೂನ್ಯ badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_ಕೂಡಿಸು(badblocks_list bb, blk_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_badblocks_list_ಕೂಡಿಸು(bb, blk);
}

ಪೂರ್ಣಾಂಕ badblocks_list_test(badblocks_list bb, blk_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_badblocks_list_iterate_begin(bb, ret);
}

ಪೂರ್ಣಾಂಕ badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_badblocks_list_iterate(iter, blk);
}

ಶೂನ್ಯ badblocks_list_iterate_ಕೊನೆ(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_ಕೊನೆ(iter);
}
