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
#پایان_اگر
#ضمن <string.h>
#ضمن <stdio.h>
#ضمن <ctype.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ضمن "../../version.h"

ساكن ثابت حرف *lib_نسخه = E2_FSPROGS_نسخه;
ساكن ثابت حرف *lib_date = E2FSPROGS_DATE;

صحيح ext2fs_parse_نسخه_string(ثابت حرف *ver_string)
{
	ثابت حرف *cp;
	صحيح نسخه = 0, dot_شمار = 0;

	لكل (cp = ver_string; *cp; cp++) {
		إذا (*cp == '.') {
			إذا (dot_شمار++)
				اكسر;
			وإلا
				استمر;
		}
		إذا (!آیا_رقم(*cp))
			اكسر;
		نسخه = (نسخه * 10) + (*cp - '0');
	}
	أرجع نسخه;
}


صحيح ext2fs_get_library_نسخه(ثابت حرف **ver_string,
			       ثابت حرف **date_string)
{
	إذا (ver_string)
		*ver_string = lib_نسخه;
	إذا (date_string)
		*date_string = lib_date;

	أرجع ext2fs_parse_نسخه_string(lib_نسخه);
}
