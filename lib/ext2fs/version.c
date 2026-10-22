/*
 * version.c --- Return the version of the ext2 library
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <stdio.h>
#ضمن <ctype.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ضمن "../../version.h"

ساكن ثابت حرف *lib_نسخو = E2_FSPROGS_نسخو;
ساكن ثابت حرف *lib_date = E2FSPROGS_DATE;

صحيح ext2fs_parse_نسخو_string(ثابت حرف *ver_string)
{
	ثابت حرف *cp;
	صحيح نسخو = 0, dot_ڳڻپ = 0;

	لكل (cp = ver_string; *cp; cp++) {
		إذا (*cp == '.') {
			إذا (dot_ڳڻپ++)
				اكسر;
			وإلا
				استمر;
		}
		إذا (!انگ_آهي(*cp))
			اكسر;
		نسخو = (نسخو * 10) + (*cp - '0');
	}
	أرجع نسخو;
}


صحيح ext2fs_get_library_نسخو(ثابت حرف **ver_string,
			       ثابت حرف **date_string)
{
	إذا (ver_string)
		*ver_string = lib_نسخو;
	إذا (date_string)
		*date_string = lib_date;

	أرجع ext2fs_parse_نسخو_string(lib_نسخو);
}
