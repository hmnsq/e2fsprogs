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
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <ctype.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#अन्तर्भाव "../../version.h"

स्थिर ध्रुव वर्ण *lib_संस्करणम् = E2_FSPROGS_संस्करणम्;
स्थिर ध्रुव वर्ण *lib_date = E2FSPROGS_DATE;

पूर्णाङ्क ext2fs_parse_संस्करणम्_string(ध्रुव वर्ण *ver_string)
{
	ध्रुव वर्ण *cp;
	पूर्णाङ्क संस्करणम् = 0, dot_गणनम् = 0;

	प्रति (cp = ver_string; *cp; cp++) {
		यदि (*cp == '.') {
			यदि (dot_गणनम्++)
				भङ्ग;
			अन्यथा
				अनुवर्त;
		}
		यदि (!अङ्कः_वा(*cp))
			भङ्ग;
		संस्करणम् = (संस्करणम् * 10) + (*cp - '0');
	}
	प्रत्यागम संस्करणम्;
}


पूर्णाङ्क ext2fs_get_library_संस्करणम्(ध्रुव वर्ण **ver_string,
			       ध्रुव वर्ण **date_string)
{
	यदि (ver_string)
		*ver_string = lib_संस्करणम्;
	यदि (date_string)
		*date_string = lib_date;

	प्रत्यागम ext2fs_parse_संस्करणम्_string(lib_संस्करणम्);
}
