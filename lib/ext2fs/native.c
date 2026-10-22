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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

પૂર્ણાંક ext2fs_native_flag(શૂન્ય)
{
#ifdef શબ્દો_BIGENDIAN
	પાછા EXT2_FLAG_SWAP_BYTES;
#નહીંતર
	પાછા 0;
#endif
}



