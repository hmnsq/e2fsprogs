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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <ctype.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#अन्तर्भाव "../../version.h"

स्थिर अचर वर्ण *lib_version = E2FSPROGS_VERSION;
स्थिर अचर वर्ण *lib_date = E2FSPROGS_DATE;

पूर्णाङ्क ext2fs_parse_version_string(अचर वर्ण *ver_string)
{
	अचर वर्ण *cp;
	पूर्णाङ्क version = 0, dot_गन्ती = 0;

	लागि (cp = ver_string; *cp; cp++) {
		यदि (*cp == '.') {
			यदि (dot_गन्ती++)
				रोक;
			अन्यथा
				जारी;
		}
		यदि (!है_अंक(*cp))
			रोक;
		version = (version * 10) + (*cp - '0');
	}
	फर्काऊ version;
}


पूर्णाङ्क ext2fs_get_library_version(अचर वर्ण **ver_string,
			       अचर वर्ण **date_string)
{
	यदि (ver_string)
		*ver_string = lib_version;
	यदि (date_string)
		*date_string = lib_date;

	फर्काऊ ext2fs_parse_version_string(lib_version);
}
