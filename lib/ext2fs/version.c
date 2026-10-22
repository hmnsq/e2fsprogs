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

#अन्तर्भाव "config.h"
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <ctype.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#अन्तर्भाव "../../version.h"

स्थिर स्थिरांक अक्षर *lib_version = E2FSPROGS_VERSION;
स्थिर स्थिरांक अक्षर *lib_date = E2FSPROGS_DATE;

पूर्णांक ext2fs_parse_version_string(स्थिरांक अक्षर *ver_string)
{
	स्थिरांक अक्षर *cp;
	पूर्णांक version = 0, dot_मोजणी = 0;

	साठी (cp = ver_string; *cp; cp++) {
		जर (*cp == '.') {
			जर (dot_मोजणी++)
				थांबा;
			अन्यथा
				सुरूठेवा;
		}
		जर (!है_अंक(*cp))
			थांबा;
		version = (version * 10) + (*cp - '0');
	}
	परत version;
}


पूर्णांक ext2fs_get_library_version(स्थिरांक अक्षर **ver_string,
			       स्थिरांक अक्षर **date_string)
{
	जर (ver_string)
		*ver_string = lib_version;
	जर (date_string)
		*date_string = lib_date;

	परत ext2fs_parse_version_string(lib_version);
}
