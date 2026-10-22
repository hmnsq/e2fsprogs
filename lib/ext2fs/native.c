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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_native_flag(ଶୂନ୍ୟ)
{
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ଫେରନ୍ତୁ EXT2_FLAG_SWAP_BYTES;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ 0;
#endif
}



