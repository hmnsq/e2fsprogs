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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "ext2_fs.h"
#include "ext2fs.h"

#include "../../version.h"

static const char *lib_version = E2FSPROGS_VERSION;
static const char *lib_date = E2FSPROGS_DATE;

số_nguyên ext2fs_parse_version_string(const char *ver_string)
{
	const char *cp;
	số_nguyên version = 0, dot_count = 0;

	cho (cp = ver_string; *cp; cp++) {
		nếu (*cp == '.') {
			nếu (dot_count++)
				break;
			không_thì
				continue;
		}
		nếu (!isdigit(*cp))
			break;
		version = (version * 10) + (*cp - '0');
	}
	trả_về version;
}


số_nguyên ext2fs_get_library_version(const char **ver_string,
			       const char **date_string)
{
	nếu (ver_string)
		*ver_string = lib_version;
	nếu (date_string)
		*date_string = lib_date;

	trả_về ext2fs_parse_version_string(lib_version);
}
