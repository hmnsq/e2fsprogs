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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

முழு ext2fs_native_flag(வெறுமை)
{
#ifdef சொற்கள்_BIGENDIAN
	திரும்பு EXT2_FLAG_SWAP_BYTES;
#இல்லையெனில்
	திரும்பு 0;
#endif
}



