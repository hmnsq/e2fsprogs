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

#समावेश "config.h"
#समावेश <stdio.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

पूर्णांक ext2fs_native_flag(व्योम)
{
#अगर_घोषित शब्दगण_BIGENDIAN
	वापस EXT2_FLAG_SWAP_BYTES;
#अन्यथा
	वापस 0;
#पूर्ण_अगर
}



