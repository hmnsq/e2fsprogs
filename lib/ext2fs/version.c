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

#அந்தர்பாவ "config.h"
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <ctype.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#அந்தர்பாவ "../../version.h"

நிலையான மாறிலி எழுத்து *lib_version = E2FSPROGS_VERSION;
நிலையான மாறிலி எழுத்து *lib_date = E2FSPROGS_DATE;

முழு ext2fs_parse_version_string(மாறிலி எழுத்து *ver_string)
{
	மாறிலி எழுத்து *cp;
	முழு version = 0, dot_மொத்தம் = 0;

	க்கு (cp = ver_string; *cp; cp++) {
		எனில் (*cp == '.') {
			எனில் (dot_மொத்தம்++)
				நிறுத்து;
			இல்லையெனில்
				தொடர்;
		}
		எனில் (!isdigit(*cp))
			நிறுத்து;
		version = (version * 10) + (*cp - '0');
	}
	திரும்பு version;
}


முழு ext2fs_get_library_version(மாறிலி எழுத்து **ver_string,
			       மாறிலி எழுத்து **date_string)
{
	எனில் (ver_string)
		*ver_string = lib_version;
	எனில் (date_string)
		*date_string = lib_date;

	திரும்பு ext2fs_parse_version_string(lib_version);
}
