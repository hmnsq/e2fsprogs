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

#અન્તર્ભાવ "config.h"
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <ctype.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#અન્તર્ભાવ "../../version.h"

સ્થિર અચલ અક્ષર *lib_version = E2FSPROGS_VERSION;
સ્થિર અચલ અક્ષર *lib_date = E2FSPROGS_DATE;

પૂર્ણાંક ext2fs_parse_version_string(અચલ અક્ષર *ver_string)
{
	અચલ અક્ષર *cp;
	પૂર્ણાંક version = 0, dot_ગણતરી = 0;

	માટે (cp = ver_string; *cp; cp++) {
		જો (*cp == '.') {
			જો (dot_ગણતરી++)
				તોડો;
			નહીંતર
				ચાલુ;
		}
		જો (!isdigit(*cp))
			તોડો;
		version = (version * 10) + (*cp - '0');
	}
	પાછા version;
}


પૂર્ણાંક ext2fs_get_library_version(અચલ અક્ષર **ver_string,
			       અચલ અક્ષર **date_string)
{
	જો (ver_string)
		*ver_string = lib_version;
	જો (date_string)
		*date_string = lib_date;

	પાછા ext2fs_parse_version_string(lib_version);
}
