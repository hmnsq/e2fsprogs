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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <ctype.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ಅನ್ತರ್ಭಾವ "../../version.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lib_ಆವೃತ್ತಿ = E2_FSPROGS_ಆವೃತ್ತಿ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lib_date = E2FSPROGS_DATE;

ಪೂರ್ಣಾಂಕ ext2fs_parse_ಆವೃತ್ತಿ_string(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ver_string)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp;
	ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ = 0, dot_ಎಣಿಕೆ = 0;

	ಗಾಗಿ (cp = ver_string; *cp; cp++) {
		ಆದರೆ (*cp == '.') {
			ಆದರೆ (dot_ಎಣಿಕೆ++)
				ನಿಲ್ಲಿಸು;
			ಇಲ್ಲದಿದ್ದರೆ
				ಮುಂದುವರಿ;
		}
		ಆದರೆ (!ಅಂಕಿಯೇ(*cp))
			ನಿಲ್ಲಿಸು;
		ಆವೃತ್ತಿ = (ಆವೃತ್ತಿ * 10) + (*cp - '0');
	}
	ಹಿಂತಿರುಗಿ ಆವೃತ್ತಿ;
}


ಪೂರ್ಣಾಂಕ ext2fs_get_library_ಆವೃತ್ತಿ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ver_string,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **date_string)
{
	ಆದರೆ (ver_string)
		*ver_string = lib_ಆವೃತ್ತಿ;
	ಆದರೆ (date_string)
		*date_string = lib_date;

	ಹಿಂತಿರುಗಿ ext2fs_parse_ಆವೃತ್ತಿ_string(lib_ಆವೃತ್ತಿ);
}
