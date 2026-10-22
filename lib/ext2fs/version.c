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

#অন্তর্ভাৱ "config.h"
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <ctype.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

#অন্তর্ভাৱ "../../version.h"

স্থির ধ্রুৱ ৱর্ণ *lib_version = E2FSPROGS_VERSION;
স্থির ধ্রুৱ ৱর্ণ *lib_date = E2FSPROGS_DATE;

পূর্ণাঙ্ক ext2fs_parse_version_string(ধ্রুৱ ৱর্ণ *ver_string)
{
	ধ্রুৱ ৱর্ণ *cp;
	পূর্ণাঙ্ক version = 0, dot_গুনতি = 0;

	বাবে (cp = ver_string; *cp; cp++) {
		যদি (*cp == '.') {
			যদি (dot_গুনতি++)
				ভঙ্গ;
			অন্যথা
				অনুৱর্ত;
		}
		যদি (!isdigit(*cp))
			ভঙ্গ;
		version = (version * 10) + (*cp - '0');
	}
	ঘূৰাই version;
}


পূর্ণাঙ্ক ext2fs_get_library_version(ধ্রুৱ ৱর্ণ **ver_string,
			       ধ্রুৱ ৱর্ণ **date_string)
{
	যদি (ver_string)
		*ver_string = lib_version;
	যদি (date_string)
		*date_string = lib_date;

	ঘূৰাই ext2fs_parse_version_string(lib_version);
}
