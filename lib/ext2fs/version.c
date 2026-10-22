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

#অন্তর্ভাব "config.h"
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <ctype.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#অন্তর্ভাব "../../version.h"

স্থির ধ্রুব বর্ণ *lib_version = E2FSPROGS_VERSION;
স্থির ধ্রুব বর্ণ *lib_date = E2FSPROGS_DATE;

পূর্ণাঙ্ক ext2fs_parse_version_string(ধ্রুব বর্ণ *ver_string)
{
	ধ্রুব বর্ণ *cp;
	পূর্ণাঙ্ক version = 0, dot_গুনতি = 0;

	প্রতি (cp = ver_string; *cp; cp++) {
		যদি (*cp == '.') {
			যদি (dot_গুনতি++)
				ভঙ্গ;
			অন্যথা
				অনুবর্ত;
		}
		যদি (!isdigit(*cp))
			ভঙ্গ;
		version = (version * 10) + (*cp - '0');
	}
	প্রত্যাগম version;
}


পূর্ণাঙ্ক ext2fs_get_library_version(ধ্রুব বর্ণ **ver_string,
			       ধ্রুব বর্ণ **date_string)
{
	যদি (ver_string)
		*ver_string = lib_version;
	যদি (date_string)
		*date_string = lib_date;

	প্রত্যাগম ext2fs_parse_version_string(lib_version);
}
