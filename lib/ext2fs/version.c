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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <ctype.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

#ଅନ୍ତର୍ଭାଵ "../../version.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *lib_version = E2FSPROGS_VERSION;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *lib_date = E2FSPROGS_DATE;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_parse_version_string(ଅଚଳ ଅକ୍ଷର *ver_string)
{
	ଅଚଳ ଅକ୍ଷର *cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା version = 0, dot_ଗଣନା = 0;

	ପାଇଁ (cp = ver_string; *cp; cp++) {
		ଯଦି (*cp == '.') {
			ଯଦି (dot_ଗଣନା++)
				ଭାଙ୍ଗ;
			ଅନ୍ୟଥା
				ଜାରି;
		}
		ଯଦି (!isdigit(*cp))
			ଭାଙ୍ଗ;
		version = (version * 10) + (*cp - '0');
	}
	ଫେରନ୍ତୁ version;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_get_library_version(ଅଚଳ ଅକ୍ଷର **ver_string,
			       ଅଚଳ ଅକ୍ଷର **date_string)
{
	ଯଦି (ver_string)
		*ver_string = lib_version;
	ଯଦି (date_string)
		*date_string = lib_date;

	ଫେରନ୍ତୁ ext2fs_parse_version_string(lib_version);
}
