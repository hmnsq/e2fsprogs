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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <ctype.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ಅನ್ತರ್ಭಾವ "../../version.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *lib_version = E2FSPROGS_VERSION;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *lib_date = E2FSPROGS_DATE;

ಪೂರ್ಣಾಙ್ಕ ext2fs_parse_version_string(ಧ್ರುವ ವರ್ಣ *ver_string)
{
	ಧ್ರುವ ವರ್ಣ *cp;
	ಪೂರ್ಣಾಙ್ಕ version = 0, dot_ಎಣಿಕೆ = 0;

	ಪ್ರತಿ (cp = ver_string; *cp; cp++) {
		ಯದಿ (*cp == '.') {
			ಯದಿ (dot_ಎಣಿಕೆ++)
				ಭಙ್ಗ;
			ಅನ್ಯಥಾ
				ಅನುವರ್ತ;
		}
		ಯದಿ (!isdigit(*cp))
			ಭಙ್ಗ;
		version = (version * 10) + (*cp - '0');
	}
	ಪ್ರತ್ಯಾಗಮ version;
}


ಪೂರ್ಣಾಙ್ಕ ext2fs_get_library_version(ಧ್ರುವ ವರ್ಣ **ver_string,
			       ಧ್ರುವ ವರ್ಣ **date_string)
{
	ಯದಿ (ver_string)
		*ver_string = lib_version;
	ಯದಿ (date_string)
		*date_string = lib_date;

	ಪ್ರತ್ಯಾಗಮ ext2fs_parse_version_string(lib_version);
}
