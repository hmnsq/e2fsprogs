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

#ضمن "config.h"
#ضمن <stdio.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

صحيح ext2fs_native_flag(فراغ)
{
#ifdef وشەکان_BIGENDIAN
	أرجع EXT2_FLAG_SWAP_BYTES;
#وإلا
	أرجع 0;
#endif
}



