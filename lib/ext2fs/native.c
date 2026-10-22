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

#include "config.h"
#include <stdio.h>

#include "ext2_fs.h"
#include "ext2fs.h"

정수 ext2fs_native_flag(void)
{
#ifdef WORDS_BIGENDIAN
	반환 EXT2_FLAG_SWAP_BYTES;
#아니면
	반환 0;
#endif
}



