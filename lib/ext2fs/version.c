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
#endif
#شامل <string.h>
#شامل <stdio.h>
#شامل <ctype.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#شامل "../../version.h"

ایستا ثابت نویسه *lib_version = E2FSPROGS_VERSION;
ایستا ثابت نویسه *lib_date = E2FSPROGS_DATE;

صحیح ext2fs_parse_version_string(ثابت نویسه *ver_string)
{
	ثابت نویسه *cp;
	صحیح version = 0, dot_شمار = 0;

	برای (cp = ver_string; *cp; cp++) {
		اگر (*cp == '.') {
			اگر (dot_شمار++)
				بشکن;
			وگرنه
				ادامه;
		}
		اگر (!isdigit(*cp))
			بشکن;
		version = (version * 10) + (*cp - '0');
	}
	بازگشت version;
}


صحیح ext2fs_get_library_version(ثابت نویسه **ver_string,
			       ثابت نویسه **date_string)
{
	اگر (ver_string)
		*ver_string = lib_version;
	اگر (date_string)
		*date_string = lib_date;

	بازگشت ext2fs_parse_version_string(lib_version);
}
