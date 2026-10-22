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

#include "config.h"
#ᱡᱩᱫᱤ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "ext2_fs.h"
#include "ext2fs.h"

#include "../../version.h"

static const var_na *lib_version = E2FSPROGS_VERSION;
static const var_na *lib_date = E2FSPROGS_DATE;

puur_naa_nka ext2fs_parse_version_string(const var_na *ver_string)
{
	const var_na *cp;
	puur_naa_nka version = 0, dot_count = 0;

	for (cp = ver_string; *cp; cp++) {
		ᱡᱩᱫᱤ (*cp == '.') {
			ᱡᱩᱫᱤ (dot_count++)
				bha_nga;
			ᱵᱟᱝ
				continue;
		}
		ᱡᱩᱫᱤ (!isdigit(*cp))
			bha_nga;
		version = (version * 10) + (*cp - '0');
	}
	ᱨᱩᱣᱟᱹᱲ version;
}


puur_naa_nka ext2fs_get_library_version(const var_na **ver_string,
			       const var_na **date_string)
{
	ᱡᱩᱫᱤ (ver_string)
		*ver_string = lib_version;
	ᱡᱩᱫᱤ (date_string)
		*date_string = lib_date;

	ᱨᱩᱣᱟᱹᱲ ext2fs_parse_version_string(lib_version);
}
