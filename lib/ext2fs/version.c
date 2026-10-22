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

#شامل "config.h"
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <string.h>
#شامل <stdio.h>
#شامل <ctype.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#شامل "../../version.h"

ساکن ثابت حرف *lib_نسخہ = E2_FSPROGS_نسخہ;
ساکن ثابت حرف *lib_date = E2FSPROGS_DATE;

صحیح ext2fs_parse_نسخہ_string(ثابت حرف *ver_string)
{
	ثابت حرف *cp;
	صحیح نسخہ = 0, dot_گنتی = 0;

	برائے (cp = ver_string; *cp; cp++) {
		اگر (*cp == '.') {
			اگر (dot_گنتی++)
				توڑو;
			ورنہ
				جاری;
		}
		اگر (!ہندسہ_ہے(*cp))
			توڑو;
		نسخہ = (نسخہ * 10) + (*cp - '0');
	}
	واپس نسخہ;
}


صحیح ext2fs_get_library_نسخہ(ثابت حرف **ver_string,
			       ثابت حرف **date_string)
{
	اگر (ver_string)
		*ver_string = lib_نسخہ;
	اگر (date_string)
		*date_string = lib_date;

	واپس ext2fs_parse_نسخہ_string(lib_نسخہ);
}
