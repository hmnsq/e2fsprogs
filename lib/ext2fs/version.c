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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <ctype.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#अन्तर्भाव "../../version.h"

स्थिर ध्रुव वर्ण *lib_version = E2FSPROGS_VERSION;
स्थिर ध्रुव वर्ण *lib_date = E2FSPROGS_DATE;

पूर्णाङ्क ext2fs_parse_version_string(ध्रुव वर्ण *ver_string)
{
	ध्रुव वर्ण *cp;
	पूर्णाङ्क version = 0, dot_गणनम् = 0;

	अत्थाय (cp = ver_string; *cp; cp++) {
		सचे (*cp == '.') {
			सचे (dot_गणनम्++)
				भङ्ग;
			अञ्ञथा
				अनुवर्त;
		}
		सचे (!isdigit(*cp))
			भङ्ग;
		version = (version * 10) + (*cp - '0');
	}
	पटिनिवत्त version;
}


पूर्णाङ्क ext2fs_get_library_version(ध्रुव वर्ण **ver_string,
			       ध्रुव वर्ण **date_string)
{
	सचे (ver_string)
		*ver_string = lib_version;
	सचे (date_string)
		*date_string = lib_date;

	पटिनिवत्त ext2fs_parse_version_string(lib_version);
}
