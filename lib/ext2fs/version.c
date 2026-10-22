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
#만약 HAVE_UNISTD_H
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

정수 ext2fs_parse_version_string(const char *ver_string)
{
	const char *cp;
	정수 version = 0, dot_count = 0;

	위해 (cp = ver_string; *cp; cp++) {
		만약 (*cp == '.') {
			만약 (dot_count++)
				break;
			아니면
				continue;
		}
		만약 (!isdigit(*cp))
			break;
		version = (version * 10) + (*cp - '0');
	}
	반환 version;
}


정수 ext2fs_get_library_version(const char **ver_string,
			       const char **date_string)
{
	만약 (ver_string)
		*ver_string = lib_version;
	만약 (date_string)
		*date_string = lib_date;

	반환 ext2fs_parse_version_string(lib_version);
}
