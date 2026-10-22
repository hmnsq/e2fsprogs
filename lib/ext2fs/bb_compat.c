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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

errcode_t badblocks_list_create(badblocks_list *ret, पूर्णाङ्क size)
{
	प्रत्यागम ext2fs_badblocks_list_create(ret, size);
}

शून्य badblocks_list_free(badblocks_list bb)
{
	ext2fs_badblocks_list_free(bb);
}

errcode_t badblocks_list_add(badblocks_list bb, blk_t blk)
{
	प्रत्यागम ext2fs_badblocks_list_add(bb, blk);
}

पूर्णाङ्क badblocks_list_test(badblocks_list bb, blk_t blk)
{
	प्रत्यागम ext2fs_badblocks_list_test(bb, blk);
}

errcode_t badblocks_list_iterate_begin(badblocks_list bb,
				       badblocks_iterate *ret)
{
	प्रत्यागम ext2fs_badblocks_list_iterate_begin(bb, ret);
}

पूर्णाङ्क badblocks_list_iterate(badblocks_iterate iter, blk_t *blk)
{
	प्रत्यागम ext2fs_badblocks_list_iterate(iter, blk);
}

शून्य badblocks_list_iterate_end(badblocks_iterate iter)
{
	ext2fs_badblocks_list_iterate_end(iter);
}
