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

#شامل "config.h"
#شامل <stdio.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

صحیح ext2fs_native_flag(تهی)
{
#ifdef واژگان_BIGENDIAN
	بازگشت EXT2_FLAG_SWAP_BYTES;
#وگرنه
	بازگشت 0;
#endif
}



