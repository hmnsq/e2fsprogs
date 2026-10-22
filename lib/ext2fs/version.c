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

#അന്തര്ഭാവ "config.h"
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <ctype.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#അന്തര്ഭാവ "../../version.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *lib_version = E2FSPROGS_VERSION;
സ്ഥിതം സ്ഥിരം അക്ഷരം *lib_date = E2FSPROGS_DATE;

പൂർണ്ണസംഖ്യ ext2fs_parse_version_string(സ്ഥിരം അക്ഷരം *ver_string)
{
	സ്ഥിരം അക്ഷരം *cp;
	പൂർണ്ണസംഖ്യ version = 0, dot_എണ്ണം = 0;

	വേണ്ടി (cp = ver_string; *cp; cp++) {
		എങ്കിൽ (*cp == '.') {
			എങ്കിൽ (dot_എണ്ണം++)
				നിർത്തുക;
			അല്ലെങ്കിൽ
				തുടരുക;
		}
		എങ്കിൽ (!isdigit(*cp))
			നിർത്തുക;
		version = (version * 10) + (*cp - '0');
	}
	തിരികെ version;
}


പൂർണ്ണസംഖ്യ ext2fs_get_library_version(സ്ഥിരം അക്ഷരം **ver_string,
			       സ്ഥിരം അക്ഷരം **date_string)
{
	എങ്കിൽ (ver_string)
		*ver_string = lib_version;
	എങ്കിൽ (date_string)
		*date_string = lib_date;

	തിരികെ ext2fs_parse_version_string(lib_version);
}
