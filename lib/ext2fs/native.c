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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

పూర్ణాంకం ext2fs_native_flag(శూన్య)
{
#నిర్వచితమైతే పదాలు_BIGENDIAN
	తిరిగి EXT2_FLAG_మార్పిడి_బైట్లు;
#లేకపోతే
	తిరిగి 0;
#అయితే_ముగింపు
}



