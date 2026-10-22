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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

पूर्णाङ्क ext2fs_native_flag(शून्य)
{
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	प्रत्यागम EXT2_FLAG_SWAP_अष्टकानि;
#अन्यथा
	प्रत्यागम 0;
#यदि_अन्तः
}



