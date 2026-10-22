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

স্থির ধ্রুবক অক্ষর *lib_version = E2FSPROGS_VERSION;
স্থির ধ্রুবক অক্ষর *lib_date = E2FSPROGS_DATE;

পূর্ণসংখ্যা ext2fs_parse_version_string(ধ্রুবক অক্ষর *ver_string)
{
	ধ্রুবক অক্ষর *cp;
	পূর্ণসংখ্যা version = 0, dot_গুনতি = 0;

	জন্য (cp = ver_string; *cp; cp++) {
		যদি (*cp == '.') {
			যদি (dot_গুনতি++)
				বিরতি;
			অন্যথা
				চালু;
		}
		যদি (!isdigit(*cp))
			বিরতি;
		version = (version * 10) + (*cp - '0');
	}
	ফেরত version;
}


পূর্ণসংখ্যা ext2fs_get_library_version(ধ্রুবক অক্ষর **ver_string,
			       ধ্রুবক অক্ষর **date_string)
{
	যদি (ver_string)
		*ver_string = lib_version;
	যদি (date_string)
		*date_string = lib_date;

	ফেরত ext2fs_parse_version_string(lib_version);
}
