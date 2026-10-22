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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

പൂർണ്ണസംഖ്യ ext2fs_native_flag(ശൂന്യം)
{
#ifdef വാക്കുകൾ_BIGENDIAN
	തിരികെ EXT2_FLAG_SWAP_BYTES;
#അല്ലെങ്കിൽ
	തിരികെ 0;
#endif
}



