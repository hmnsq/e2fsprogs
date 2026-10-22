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
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <stdio.h>
#ضمن <ctype.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ضمن "../../version.h"

ساكن ثابت حرف *lib_version = E2FSPROGS_VERSION;
ساكن ثابت حرف *lib_date = E2FSPROGS_DATE;

صحيح ext2fs_parse_version_string(ثابت حرف *ver_string)
{
	ثابت حرف *cp;
	صحيح version = 0, dot_count = 0;

	עבור (cp = ver_string; *cp; cp++) {
		אם (*cp == '.') {
			אם (dot_count++)
				اكسر;
			אחרת
				استمر;
		}
		אם (!isdigit(*cp))
			اكسر;
		version = (version * 10) + (*cp - '0');
	}
	החזר version;
}


صحيح ext2fs_get_library_version(ثابت حرف **ver_string,
			       ثابت حرف **date_string)
{
	אם (ver_string)
		*ver_string = lib_version;
	אם (date_string)
		*date_string = lib_date;

	החזר ext2fs_parse_version_string(lib_version);
}
