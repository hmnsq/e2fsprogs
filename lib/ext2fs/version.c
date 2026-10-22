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
#ако HAVE_UNISTD_H
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

цяло ext2fs_parse_version_string(const char *ver_string)
{
	const char *cp;
	цяло version = 0, dot_count = 0;

	за (cp = ver_string; *cp; cp++) {
		ако (*cp == '.') {
			ако (dot_count++)
				break;
			иначе
				continue;
		}
		ако (!isdigit(*cp))
			break;
		version = (version * 10) + (*cp - '0');
	}
	върни version;
}


цяло ext2fs_get_library_version(const char **ver_string,
			       const char **date_string)
{
	ако (ver_string)
		*ver_string = lib_version;
	ако (date_string)
		*date_string = lib_date;

	върни ext2fs_parse_version_string(lib_version);
}
