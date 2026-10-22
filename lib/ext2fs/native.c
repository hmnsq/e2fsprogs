/*
 * native.c --- returns the ext2_flag for a native byte order
 *
 * Copyright (C) 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

পূর্ণাঙ্ক ext2fs_native_flag(শূন্য)
{
#ifdef শব্দগুলি_BIGENDIAN
	ঘূৰাই EXT2_FLAG_SWAP_BYTES;
#অন্যথা
	ঘূৰাই 0;
#endif
}



