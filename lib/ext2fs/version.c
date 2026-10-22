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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "../../version.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *lib_ਸੰਸਕਰਣ = E2_FSPROGS_ਸੰਸਕਰਣ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *lib_date = E2FSPROGS_DATE;

ਪੂਰਨ ext2fs_parse_ਸੰਸਕਰਣ_string(ਅਚਲ ਅੱਖਰ *ver_string)
{
	ਅਚਲ ਅੱਖਰ *cp;
	ਪੂਰਨ ਸੰਸਕਰਣ = 0, dot_ਗਿਣਤੀ = 0;

	ਲਈ (cp = ver_string; *cp; cp++) {
		ਜੇ (*cp == '.') {
			ਜੇ (dot_ਗਿਣਤੀ++)
				ਰੋਕੋ;
			ਨਹੀਂਤਾਂ
				ਜਾਰੀ;
		}
		ਜੇ (!ਅੰਕ_ਹੈ(*cp))
			ਰੋਕੋ;
		ਸੰਸਕਰਣ = (ਸੰਸਕਰਣ * 10) + (*cp - '0');
	}
	ਵਾਪਸ ਸੰਸਕਰਣ;
}


ਪੂਰਨ ext2fs_get_library_ਸੰਸਕਰਣ(ਅਚਲ ਅੱਖਰ **ver_string,
			       ਅਚਲ ਅੱਖਰ **date_string)
{
	ਜੇ (ver_string)
		*ver_string = lib_ਸੰਸਕਰਣ;
	ਜੇ (date_string)
		*date_string = lib_date;

	ਵਾਪਸ ext2fs_parse_ਸੰਸਕਰਣ_string(lib_ਸੰਸਕਰਣ);
}
