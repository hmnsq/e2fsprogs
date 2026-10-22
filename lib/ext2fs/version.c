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
#جے_انت
#ضمن <string.h>
#ضمن <stdio.h>
#ضمن <ctype.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ضمن "../../version.h"

ساكن ثابت حرف *lib_نسخہ = E2_FSPROGS_نسخہ;
ساكن ثابت حرف *lib_date = E2FSPROGS_DATE;

صحيح ext2fs_parse_نسخہ_string(ثابت حرف *ver_string)
{
	ثابت حرف *cp;
	صحيح نسخہ = 0, dot_گنتی = 0;

	لكل (cp = ver_string; *cp; cp++) {
		إذا (*cp == '.') {
			إذا (dot_گنتی++)
				اكسر;
			وإلا
				استمر;
		}
		إذا (!ہندسہ_اے(*cp))
			اكسر;
		نسخہ = (نسخہ * 10) + (*cp - '0');
	}
	أرجع نسخہ;
}


صحيح ext2fs_get_library_نسخہ(ثابت حرف **ver_string,
			       ثابت حرف **date_string)
{
	إذا (ver_string)
		*ver_string = lib_نسخہ;
	إذا (date_string)
		*date_string = lib_date;

	أرجع ext2fs_parse_نسخہ_string(lib_نسخہ);
}
