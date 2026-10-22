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
#ถ้า HAVE_UNISTD_H
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

จำนวนเต็ม ext2fs_parse_version_string(const var_na *ver_string)
{
	const var_na *cp;
	จำนวนเต็ม version = 0, dot_count = 0;

	สำหรับ (cp = ver_string; *cp; cp++) {
		ถ้า (*cp == '.') {
			ถ้า (dot_count++)
				bha_nga;
			มิฉะนั้น
				continue;
		}
		ถ้า (!isdigit(*cp))
			bha_nga;
		version = (version * 10) + (*cp - '0');
	}
	คืน version;
}


จำนวนเต็ม ext2fs_get_library_version(const var_na **ver_string,
			       const var_na **date_string)
{
	ถ้า (ver_string)
		*ver_string = lib_version;
	ถ้า (date_string)
		*date_string = lib_date;

	คืน ext2fs_parse_version_string(lib_version);
}
