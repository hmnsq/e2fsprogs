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

#समावेश "config.h"
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <stdio.h>
#समावेश <ctype.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

#समावेश "../../version.h"

अटल स्थिर अक्षर *lib_version = E2FSPROGS_VERSION;
अटल स्थिर अक्षर *lib_date = E2FSPROGS_DATE;

पूर्णांक ext2fs_parse_version_string(स्थिर अक्षर *ver_string)
{
	स्थिर अक्षर *cp;
	पूर्णांक version = 0, dot_गिनती = 0;

	क्रम (cp = ver_string; *cp; cp++) {
		अगर (*cp == '.') {
			अगर (dot_गिनती++)
				अवरोध;
			अन्यथा
				जारी;
		}
		अगर (!है_अंक(*cp))
			अवरोध;
		version = (version * 10) + (*cp - '0');
	}
	वापस version;
}


पूर्णांक ext2fs_get_library_version(स्थिर अक्षर **ver_string,
			       स्थिर अक्षर **date_string)
{
	अगर (ver_string)
		*ver_string = lib_version;
	अगर (date_string)
		*date_string = lib_date;

	वापस ext2fs_parse_version_string(lib_version);
}
